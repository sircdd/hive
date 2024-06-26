from __future__ import annotations

import pytest

import test_tools as tt
from hive_local_tools import run_for
from hive_local_tools.constants import TIME_REQUIRED_TO_DECLINE_VOTING_RIGHTS
from hive_local_tools.functional.python.hf28.constants import PROXY_ACCOUNT, VOTER_ACCOUNT
from hive_local_tools.functional.python.operation import get_virtual_operations
from schemas.operations.virtual import ProxyClearedOperation


@run_for("testnet")
def test_proxy_before_waiving_voting_rights(prepare_environment: tuple[tt.InitNode, tt.Wallet]) -> None:
    node, wallet = prepare_environment

    wallet.api.set_voting_proxy(VOTER_ACCOUNT, PROXY_ACCOUNT)

    wallet.api.decline_voting_rights(VOTER_ACCOUNT, True)
    assert (voter := node.api.wallet_bridge.get_account(VOTER_ACCOUNT)) is not None
    assert voter.proxy == PROXY_ACCOUNT
    node.wait_number_of_blocks(TIME_REQUIRED_TO_DECLINE_VOTING_RIGHTS)

    assert len(get_virtual_operations(node, ProxyClearedOperation)) == 1
    assert (voter := node.api.wallet_bridge.get_account(VOTER_ACCOUNT)) is not None
    assert voter.proxy == ""

    node.restart(time_control=tt.OffsetTimeControl(offset="+25h"))
    assert (proxy := node.api.wallet_bridge.get_account(PROXY_ACCOUNT)) is not None
    assert not any(proxy.proxied_vsf_votes)


@run_for("testnet")
def test_set_proxy_after_waiving_voting_rights(prepare_environment: tuple[tt.InitNode, tt.Wallet]) -> None:
    node, wallet = prepare_environment

    wallet.api.decline_voting_rights(VOTER_ACCOUNT, True)
    node.wait_number_of_blocks(TIME_REQUIRED_TO_DECLINE_VOTING_RIGHTS)

    with pytest.raises(tt.exceptions.CommunicationError) as exception:
        wallet.api.set_voting_proxy(VOTER_ACCOUNT, PROXY_ACCOUNT)

    assert "Account has declined the ability to vote and cannot proxy votes." in exception.value.error


@run_for("testnet")
def test_set_proxy_when_decline_voting_rights_is_in_progress(
    prepare_environment: tuple[tt.InitNode, tt.Wallet]
) -> None:
    node, wallet = prepare_environment

    head_block_number = wallet.api.decline_voting_rights(VOTER_ACCOUNT, True)["block_num"]
    node.wait_for_block_with_number(head_block_number + (TIME_REQUIRED_TO_DECLINE_VOTING_RIGHTS // 2))
    wallet.api.set_voting_proxy(VOTER_ACCOUNT, PROXY_ACCOUNT)
    node.wait_for_block_with_number(head_block_number + TIME_REQUIRED_TO_DECLINE_VOTING_RIGHTS)

    assert len(get_virtual_operations(node, ProxyClearedOperation)) == 1
    node.restart(time_control=tt.OffsetTimeControl(offset="+25h"))

    assert (voter := node.api.wallet_bridge.get_account(VOTER_ACCOUNT)) is not None
    assert voter.proxy == ""

    assert (proxy := node.api.wallet_bridge.get_account(PROXY_ACCOUNT)) is not None
    assert not any(proxy.proxied_vsf_votes)


@run_for("testnet")
def test_proxied_vsf_votes_when_principal_account_declined_its_voting_rights(
    prepare_environment: tuple[tt.InitNode, tt.Wallet]
) -> None:
    node, wallet = prepare_environment

    wallet.api.set_voting_proxy(VOTER_ACCOUNT, PROXY_ACCOUNT)

    node.wait_for_irreversible_block()
    node.restart(time_control=tt.OffsetTimeControl(offset="+25h", speed_up_rate=5))

    assert (proxy := node.api.wallet_bridge.get_account(PROXY_ACCOUNT)) is not None
    assert int(proxy.proxied_vsf_votes[0]) > 0

    wallet.api.decline_voting_rights(VOTER_ACCOUNT, True)
    node.wait_number_of_blocks(TIME_REQUIRED_TO_DECLINE_VOTING_RIGHTS)

    assert (voter := node.api.wallet_bridge.get_account(VOTER_ACCOUNT)) is not None
    assert voter.proxy == ""
    assert len(get_virtual_operations(node, ProxyClearedOperation)) == 1

    assert (proxy := node.api.wallet_bridge.get_account(PROXY_ACCOUNT)) is not None
    assert not any(proxy.proxied_vsf_votes)
