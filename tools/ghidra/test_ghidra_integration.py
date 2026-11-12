#!/usr/bin/env python3
"""Offline tests for the lean Ghidra comprehension client (no live Ghidra).

Run:  python3 tests/ghidra/test_ghidra_integration.py
  or: python3 -m unittest discover -s tests/ghidra
"""

import sys
import unittest
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent.parent
sys.path.insert(0, str(REPO_ROOT))

from tools.ghidra.client import (  # noqa: E402
    GhidraClient,
    GhidraError,
    decompiled_text,
)


class _FakeResp:
    def __init__(self, text, status=200):
        self.text = text
        self.status_code = status


class _FakeSession:
    """Records the last request and returns a canned response."""

    def __init__(self, text='{"ok": true}', status=200):
        self.headers = {}
        self._text = text
        self._status = status
        self.last = None

    def request(self, method, url, params=None, timeout=None):
        self.last = {"method": method, "url": url, "params": params}
        return _FakeResp(self._text, self._status)


def _client(session):
    return GhidraClient(base_url="http://x:8089", session=session)


class TestClient(unittest.TestCase):
    def test_get_uses_query_params_and_drops_none(self):
        sess = _FakeSession('{"ok": true}')
        gc = _client(sess)
        self.assertEqual(gc.decompile("0x80042304"), {"ok": True})
        self.assertEqual(sess.last["method"], "GET")
        self.assertEqual(sess.last["params"], {"address": "0x80042304"})  # program dropped

    def test_url_built_from_base(self):
        sess = _FakeSession()
        _client(sess).disassemble("0x80041000")
        self.assertEqual(sess.last["url"], "http://x:8089/disassemble_function")

    def test_non_json_response_returns_text(self):
        gc = _client(_FakeSession(text="Connected: plugin running"))
        self.assertEqual(gc.check_connection(), "Connected: plugin running")

    def test_http_error_raises(self):
        gc = _client(_FakeSession(text="boom", status=500))
        with self.assertRaises(GhidraError):
            gc.get_version()

    def test_write_endpoints_are_refused(self):
        # This client is read-only: anything outside READ_ENDPOINTS is blocked.
        gc = _client(_FakeSession())
        with self.assertRaises(GhidraError):
            gc.get("rename_function", old_name="a", new_name="b")

    def test_is_alive_swallows_errors(self):
        class _Boom:
            headers = {}

            def request(self, *a, **k):
                raise ConnectionError("nope")

        self.assertFalse(_client(_Boom()).is_alive())

    def test_decompiled_text_extraction(self):
        self.assertEqual(decompiled_text("void f(){}"), "void f(){}")
        self.assertEqual(decompiled_text({"decompiled": "void g(){}"}), "void g(){}")
        # falls back to JSON dump when no known key is present
        self.assertIn("weird", decompiled_text({"weird": 1}))


if __name__ == "__main__":
    unittest.main()
