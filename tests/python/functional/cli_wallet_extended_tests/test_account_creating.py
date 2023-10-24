from __future__ import annotations

import pytest

import test_tools as tt


def test_account_creation(wallet: tt.Wallet) -> None:
    wallet.api.create_account("initminer", "alice", "{}")
    assert wallet.api.list_accounts("a", 1) == ["alice"]


@pytest.mark.parametrize(
    ("account_name", "expected_error_message"),
    [
        (
            "too-long-account-name",
            "Account name 'too-long-account-name' is too long. Use maximum of 16 characters.",
        ),
        (
            "to",
            "Account name 'to' is too short. Use at least 3 characters.",
        ),
        (
            "...",
            "Account name '...' is not valid. Please follow the RFC 1035 rules.",
        ),
    ],
)
def test_creation_of_account_with_invalid_name(
    wallet: tt.Wallet, account_name: str, expected_error_message: str
) -> None:
    with pytest.raises(tt.exceptions.CommunicationError) as exception:
        wallet.api.create_account("initminer", account_name, "{}")

    assert expected_error_message in exception.value.error
