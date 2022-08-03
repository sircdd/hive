from ..local_tools import run_for


@run_for('testnet', 'mainnet_5m', 'mainnet_64m')
def test_get_account_reputations(prepared_node):
    prepared_node.api.condenser.get_account_reputations('', 100)
