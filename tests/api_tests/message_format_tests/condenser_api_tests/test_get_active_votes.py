import test_tools as tt

from ..local_tools import run_for


@run_for('testnet', 'mainnet_5m', 'mainnet_64m')
def test_get_active_votes(prepared_node, should_prepare):
    if should_prepare:
        wallet = tt.Wallet(attach_to=prepared_node)
        wallet.create_account('acidyo', hives=tt.Asset.Test(100), vests=tt.Asset.Test(100))
        wallet.api.post_comment('acidyo', 'drew-an-avatar-signature-for-my-posts', '', 'test-parent-permlink', 'test-title', 'test-body', '{}')
        wallet.api.vote('initminer', 'acidyo', 'drew-an-avatar-signature-for-my-posts', 100, broadcast=True)
    prepared_node.api.condenser.get_active_votes('acidyo', 'drew-an-avatar-signature-for-my-posts')