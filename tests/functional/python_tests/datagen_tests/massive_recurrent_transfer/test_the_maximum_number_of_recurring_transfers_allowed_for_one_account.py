import math
from pathlib import Path

import test_tools as tt

from hive_local_tools.functional.python.datagen.massive_recurrent_transfer import ReplayedNodeMaker


def test_the_maximum_number_of_recurring_transfers_allowed_for_one_account(replayed_node: ReplayedNodeMaker):
    """
    Test scenario: block log that was replayed contains ordered recurrent transfers.
      1) replay block_log it contains outstanding recurring transfers,
      2) wait for all recurrent transfers to be processed,
      3) assert if there is any overdue recurring transfer.
    """
    block_log_directory = Path(__file__).parent / "block_logs/block_log_recurrent_transfer_everyone_to_everyone"

    with open(block_log_directory / "timestamp", encoding='utf-8') as file:
        timestamp = tt.Time.parse(file.read())

    # during this replay, before entering live mode node processes a lots of recurrent transfers, therefore the timeout has been increased.
    replayed_node = replayed_node(
        block_log_directory,
        absolute_start_time=timestamp + tt.Time.days(2),
        time_multiplier=90,
        timeout=240
    )
    wallet = tt.Wallet(attach_to=replayed_node)

    all_accounts_names = wallet.list_accounts()

    tt.logger.info(f"start waiting, headblock: {replayed_node.get_last_block_number()}")
    replayed_node.wait_number_of_blocks(math.ceil(len(all_accounts_names) * 255 / 1000))
    tt.logger.info(f"finish waiting, headblock: {replayed_node.get_last_block_number()}")

    for account_name in all_accounts_names:
        assert replayed_node.api.wallet_bridge.find_recurrent_transfers(account_name) == []
