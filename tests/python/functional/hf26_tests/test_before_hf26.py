from __future__ import annotations

from typing import TYPE_CHECKING

from hive_local_tools.functional.python.hf26 import hf26_operation_failed, legacy_operation_passed, prepare_wallets

if TYPE_CHECKING:
    import test_tools as tt


def test_before_hf26(node_hf25: tt.InitNode) -> None:
    wallet_legacy, wallet_hf26 = prepare_wallets(node_hf25)

    legacy_operation_passed(wallet_legacy)
    hf26_operation_failed(wallet_hf26)
