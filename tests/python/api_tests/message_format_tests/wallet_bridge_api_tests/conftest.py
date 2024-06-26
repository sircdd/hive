from __future__ import annotations

import pytest

import test_tools as tt


def pytest_configure(config):
    config.addinivalue_line("markers", "enabled_plugins: Enabled plugins in node from `node` fixture")


@pytest.fixture()
def wallet(node: tt.InitNode) -> tt.Wallet:
    return tt.Wallet(attach_to=node)
