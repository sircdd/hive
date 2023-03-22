import os
import sys
from pathlib import Path
from typing import Dict, Iterable, List, Callable, Optional, Tuple, Any
from functools import partial

import test_tools as tt
from .complex_networks_helper_functions import connect_sub_networks
import shared_tools.networks_architecture as networks

class sql_preparer:
    def __init__(self, network_under_test: int, node_under_test_name: str, session: Any = None) -> None:
        self.network_under_test     = network_under_test
        self.node_under_test_name   = node_under_test_name
        self.session                = session

    def prepare(self, builder: networks.NetworksBuilder):
        node_under_test = builder.networks[self.network_under_test].node(self.node_under_test_name)
        node_under_test.config.plugin.append('sql_serializer')
        node_under_test.config.psql_url = str(self.db_name())

    def db_name(self) -> Any:
        return None if self.session is None else self.session.get_bind().url

    def node(self, builder) -> Any:
        return builder.networks[self.network_under_test].node(self.node_under_test_name)

def get_time_offset_from_file(file: Path) -> str:
    with open(file, "r", encoding="UTF-8") as file:
        return file.read().strip()


def get_relative_time_offset_from_timestamp(timestamp: str) -> str:
    delta = tt.Time.now(serialize=False) - tt.Time.parse(timestamp)
    delta += tt.Time.seconds(5)  # Node starting and entering live mode takes some time to complete
    return f"-{delta.total_seconds():.3f}s"


def init_network(init_node, all_witness_names: List[str], key: Optional[str] = None, block_log_directory_name: Optional[Path] = None, desired_blocklog_length: Optional[int] = None) -> None:
    tt.logger.info("Attaching wallets...")
    tt.logger.info(f"Witnesses: {', '.join(all_witness_names)}")
    wallet = tt.Wallet(attach_to=init_node)
    # We are waiting here for block 43, because witness participation is counting
    # by dividing total produced blocks in last 128 slots by 128. When we were waiting
    # too short, for example 42 blocks, then participation equals 42 / 128 = 32.81%.
    # It is not enough, because 33% is required. 43 blocks guarantee, that this
    # requirement is always fulfilled (43 / 128 = 33.59%, which is greater than 33%).
    tt.logger.info("Wait for block 43 (to fulfill required 33% of witness participation)")
    init_node.wait_for_block_with_number(43)

    # Prepare witnesses on blockchain
    with wallet.in_single_transaction():
        for name in all_witness_names:
            if key is None:
                wallet.api.create_account("initminer", name, "")
            else:
                wallet.api.create_account_with_keys("initminer", name, "", key, key, key, key)
    with wallet.in_single_transaction():
        for name in all_witness_names:
            wallet.api.transfer_to_vesting("initminer", name, tt.Asset.Test(1000))
    with wallet.in_single_transaction():
        for name in all_witness_names:
            wallet.api.update_witness(
                name,
                "https://" + name,
                tt.Account(name).public_key,
                {"account_creation_fee": tt.Asset.Test(3), "maximum_block_size": 65536, "sbd_interest_rate": 0},
            )
    tt.logger.info("Wait 21 blocks to schedule newly created witnesses into future slate")
    init_node.wait_number_of_blocks(21)

    future_witnesses = init_node.api.database.get_active_witnesses(include_future=True)["future_witnesses"]
    tt.logger.info(f"Future witnesses after voting: {future_witnesses}")
    assert len(future_witnesses) == 21

    tt.logger.info("Wait 21 blocks for future slate to become active slate")
    init_node.wait_number_of_blocks(21)

    active_witnesses = init_node.api.database.get_active_witnesses()["witnesses"]
    tt.logger.info(f"Witness state after voting: {active_witnesses}")
    assert len(active_witnesses) == 21

    # Reason of this wait is to enable moving forward of irreversible block
    tt.logger.info("Wait 21 blocks (when every witness sign at least one block)")
    init_node.wait_number_of_blocks(21)

    result = wallet.api.info()
    irreversible = result["last_irreversible_block_num"]
    head = result["head_block_num"]
    tt.logger.info(f"Network prepared, irreversible block: {irreversible}, head block: {head}")

    if desired_blocklog_length is not None:
        while irreversible < desired_blocklog_length:
            init_node.wait_number_of_blocks(1)
            result = wallet.api.info()
            irreversible = result["last_irreversible_block_num"]
            tt.logger.info(
                f"Generating block_log of length: {desired_blocklog_length}, "
                f"current irreversible: {result['last_irreversible_block_num']}, "
                f"current head block: {result['head_block_num']}"
            )

    result = wallet.api.info()
    head_block_num = result["head_block_number"]
    timestamp = init_node.api.block.get_block(block_num=head_block_num)["block"]["timestamp"]
    tt.logger.info(f"head block timestamp: {timestamp}")

    # If a directory of `block_log` is given then it"s possible to save 2 files:
    # 1) "block_log" file that contains whole saved network
    # 2) "timestamp" file that contains information about time what should be set by `libfaketime` library
    if block_log_directory_name is not None:
        # Prepare dedicated directory
        if block_log_directory_name.exists():
            (block_log_directory_name / "block_log").unlink(missing_ok=True)
        else:
            Path.mkdir(block_log_directory_name)

        # Copy newly created "block_log" into dedicated directory
        init_node.close()
        init_node.block_log.copy_to(block_log_directory_name)

        # Create "timestamp file"
        with (block_log_directory_name / "timestamp").open(mode="w", encoding="UTF-8") as file_handle:
            file_handle.write(f"{timestamp}")


def modify_time_offset(old_iso_date: str, offset_in_seconds: int) -> str:
    new_iso_date = tt.Time.serialize(tt.Time.parse(old_iso_date) - tt.Time.seconds(offset_in_seconds))
    tt.logger.info(f"old date: {old_iso_date} new date(after time offset): {new_iso_date}")

    time_offset = get_relative_time_offset_from_timestamp(new_iso_date)

    return time_offset


def run_networks(networks: Iterable[tt.Network], blocklog_directory: Path, time_offsets: Optional[Iterable[str]] = None) -> None:
    if blocklog_directory is not None:
        timestamp = get_time_offset_from_file((blocklog_directory / "timestamp"))
        time_offset = get_relative_time_offset_from_timestamp(timestamp)
        block_log = tt.BlockLog(blocklog_directory / "block_log")
        tt.logger.info(f"'block_log' directory: {blocklog_directory} timestamp: {timestamp} time_offset: {time_offset}")
    else:
        tt.logger.info(f"'block_log' directory hasn't been defined")

    tt.logger.info("Running nodes...")

    connect_sub_networks(networks)

    nodes = [node for network in networks for node in network.nodes]

    info_nodes = ", ".join(str(node) for node in nodes)
    tt.logger.info(f"Following nodes exist in whole network: {info_nodes}")
    
    allow_external_time_offsets = time_offsets is not None and len(time_offsets) == len(nodes)
    tt.logger.info(f"External time offsets: {'enabled' if allow_external_time_offsets else 'disabled'}")

    if blocklog_directory is not None:
        nodes[0].run(wait_for_live=False, replay_from=block_log, time_offset=modify_time_offset(timestamp, time_offsets[0]) if allow_external_time_offsets else time_offset)
    else:
        nodes[0].run(wait_for_live=False)
    init_node_p2p_endpoint = nodes[0].p2p_endpoint
    cnt_node = 1
    for node in nodes[1:]:
        node.config.p2p_seed_node.append(init_node_p2p_endpoint)
        if blocklog_directory is not None:
            node.run(wait_for_live=False, replay_from=block_log, time_offset=modify_time_offset(timestamp, time_offsets[cnt_node]) if allow_external_time_offsets else time_offset)
        else:
            node.run(wait_for_live=False)
        cnt_node += 1

    for network in networks:
        network.is_running = True

    for node in nodes:
        tt.logger.debug(f"Waiting for {node} to be live...")
        node.wait_for_live_mode(tt.InitNode.DEFAULT_WAIT_FOR_LIVE_TIMEOUT)


def display_info(wallet) -> None:
    # Network should be set up at this time, with 21 active witnesses, enough participation rate
    # and irreversible block number lagging behind around 15-20 blocks head block number
    result = wallet.api.info()
    irreversible = result["last_irreversible_block_num"]
    head = result["head_block_num"]
    tt.logger.info(f"Network prepared, irreversible block: {irreversible}, head block: {head}")


def prepare_nodes(sub_networks_sizes: list) -> list:
    assert len(sub_networks_sizes) > 0, "At least 1 sub-network is required"

    cnt = 0
    all_witness_names = []
    sub_networks = []
    init_node = None

    for sub_networks_size in sub_networks_sizes:
        tt.logger.info(f"Preparing sub-network nr: {cnt} that consists of {sub_networks_size} witnesses")

        witness_names = [f"witness-{cnt}-{i}" for i in range(sub_networks_size)]
        all_witness_names += witness_names

        sub_network = tt.Network()
        if cnt == 0:
            init_node = tt.InitNode(network=sub_network)
        sub_networks.append(sub_network)
        tt.WitnessNode(witnesses=witness_names, network=sub_network)
        tt.ApiNode(network=sub_network)

        cnt += 1
    return sub_networks, init_node, all_witness_names


def generate_networks(architecture: networks.NetworksArchitecture, block_log_directory_name: Optional[Path] = None, before_action: Optional[Callable[[], networks.NetworksBuilder]] = None, desired_blocklog_length: Optional[int] = None) -> Dict:
    builder = networks.NetworksBuilder()
    builder.build(architecture)

    if before_action is not None:
        before_action(builder)

    run_networks(builder.networks, None)

    initminer_public_key = "TST6LLegbAgLAy28EHrffBVuANFWcFgmqRMW13wBmTExqFE9SCkg4"
    init_network(builder.init_node, builder.witness_names, initminer_public_key, block_log_directory_name, desired_blocklog_length)

    return None


def launch_networks(architecture: networks.NetworksArchitecture, block_log_directory_name: Optional[Path] = None, time_offsets: Optional[Iterable[int]] = None, before_action: Optional[Callable[[], networks.NetworksBuilder]] = None) -> Dict:
    builder = networks.NetworksBuilder()
    builder.build(architecture)

    if before_action is not None:
        before_action(builder)

    run_networks(builder.networks, block_log_directory_name, time_offsets)

    builder.init_wallet = tt.Wallet(attach_to=builder.init_node)

    display_info(builder.init_wallet)

    return builder


def generate_or_launch(architecture: networks.NetworksArchitecture, block_log_directory_name: Optional[Path] = None, time_offsets: Optional[Iterable[int]] = None, before_action: Optional[Callable[[], networks.NetworksBuilder]] = None) -> Dict:
    if allow_generate_block_log():
        assert block_log_directory_name is not None, "Name of directory with block_log file must be given"
        tt.logger.info(f"New `block_log` generation: {block_log_directory_name}")
        return generate_networks(architecture, block_log_directory_name, before_action)

    return launch_networks(architecture, block_log_directory_name, time_offsets, before_action)


def allow_generate_block_log() -> bool:
    status = os.environ.get("GENERATE_NEW_BLOCK_LOG", None)
    if status is None:
        return False
    return int(status) == 1


def before_run_network(builder: networks.NetworksBuilder, preparers: Iterable[sql_preparer]):
    for preparer in preparers:
        preparer.prepare(builder)

    for node in builder.nodes:
        node.config.log_logger = '{"name":"default","level":"debug","appender":"stderr,p2p"} '\
                                 '{"name":"user","level":"debug","appender":"stderr,p2p"} '\
                                 '{"name":"chainlock","level":"debug","appender":"p2p"} '\
                                 '{"name":"sync","level":"debug","appender":"p2p"} '\
                                 '{"name":"p2p","level":"debug","appender":"p2p"}'


def prepare_database( database, name: str = "haf_block_log") -> Any:
    return database(f"postgresql:///{name}")


def run_whole_network(architecture: networks.NetworksArchitecture, block_log_directory_name: Path = None, time_offsets: Iterable[int] = None, preparers: Iterable[sql_preparer] = None) -> Tuple[networks.NetworksBuilder, Any]:
    before_action = None
    if preparers is not None:
        before_action = partial( before_run_network, preparers = preparers)

    builder = generate_or_launch(architecture, block_log_directory_name, time_offsets, before_action)

    if builder is None:
        tt.logger.info(f"Generating 'block_log' enabled. Exiting...")
        sys.exit(1)

    return builder


def prepare_network(architecture: networks.NetworksArchitecture, block_log_directory_name: Path = None, time_offsets: Iterable[int] = None) -> networks.NetworksBuilder:
    return run_whole_network(architecture, block_log_directory_name, time_offsets)


def prepare_network_with_1_session(database, architecture: networks.NetworksArchitecture, block_log_directory_name: Path = None, time_offsets: Iterable[int] = None, preparer: sql_preparer = None) -> Tuple[networks.NetworksBuilder, Any]:
    preparer.session = prepare_database(database)
    return run_whole_network(architecture, block_log_directory_name, time_offsets, [preparer]), preparer.session


def prepare_network_with_2_sessions(database, architecture: networks.NetworksArchitecture, block_log_directory_name: Path = None, time_offsets: Iterable[int] = None, preparers: Iterable[sql_preparer] = None) -> Tuple[networks.NetworksBuilder, Any]:
    preparers[0].session = prepare_database(database)
    preparers[1].session = prepare_database(database, "haf_block_log_ref")
    return run_whole_network(architecture, block_log_directory_name, time_offsets, preparers), [preparers[0].session, preparers[1].session]


def prepare_node_with_database(database) -> Tuple[tt.ApiNode, Any, Any]:
    config = {
        "networks": [
                        {
                            "ApiNode"   : True,
                        }
                    ]
    }
    architecture = networks.NetworksArchitecture()
    architecture.load(config)

    builder = networks.NetworksBuilder()
    builder.build(architecture, True)

    network_under_test      = 0
    node_under_test_name    = "ApiNode0"

    preparer = sql_preparer(network_under_test, node_under_test_name, prepare_database(database))
    preparer.prepare(builder)

    return preparer.node(builder), preparer.session, preparer.db_name()


def prepare_time_offsets(limit: int):
    time_offsets: int = []

    cnt = 0
    for i in range(limit):
        time_offsets.append(cnt % 3 + 1)
        cnt += 1

    result = ",".join(str(time_offset) for time_offset in time_offsets)
    tt.logger.info( f"Generated: {result}" )

    return time_offsets


def create_block_log_directory_name(block_log_directory_name: str):
    return Path(__file__).parent.absolute() / "block_logs" / block_log_directory_name
