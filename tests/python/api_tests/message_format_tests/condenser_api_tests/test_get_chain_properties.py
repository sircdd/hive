from __future__ import annotations

from hive_local_tools import run_for


@run_for("testnet", "mainnet_5m", "live_mainnet")
def test_get_chain_properties(node):
    node.api.condenser.get_chain_properties()
