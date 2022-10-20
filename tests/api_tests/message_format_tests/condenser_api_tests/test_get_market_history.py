import test_tools as tt

from ..local_tools import date_from_now, run_for


@run_for('testnet', 'mainnet_5m', 'mainnet_64m')
def test_get_market_history(prepared_node, should_prepare):
    if should_prepare:
        wallet = tt.Wallet(attach_to=prepared_node)
        wallet.create_account('alice', hives=tt.Asset.Test(100), vests=tt.Asset.Test(100))
        wallet.api.create_order('alice', 0, tt.Asset.Test(50), tt.Asset.Tbd(5), False, 3600)
        wallet.api.create_order('initminer', 1, tt.Asset.Tbd(50), tt.Asset.Test(100), False, 3600)
    prepared_node.api.condenser.get_market_history(3600, date_from_now(weeks=-100), date_from_now(weeks=1))