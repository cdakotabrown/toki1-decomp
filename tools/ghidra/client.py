#!/usr/bin/env python3
"""Lean, read-only HTTP client for a running ghidra-mcp plugin.

This is a *comprehension oracle*, not a matcher: it fetches Ghidra's read-only
views (decompilation, disassembly, xrefs, call graph) over the plugin's HTTP
server on 127.0.0.1:8089 so you can eyeball what a gnarly function does. It
does NOT produce byte-perfect C -- the m2c / decomp-permuter / ido_diff
pipeline owns that. See tools/ghidra/README.md.

Self-contained: only GET endpoints, no write operations, no external spec.
Works offline in the sense that it fails gracefully with a clear message when
the plugin is not running.
"""

from __future__ import annotations

import json
import os
from typing import Any

try:  # requests is in tools/requirements-python.txt.
    import requests
except ImportError:  # pragma: no cover
    requests = None  # type: ignore[assignment]

DEFAULT_URL = os.environ.get("GHIDRA_MCP_URL", "http://127.0.0.1:8089")
DEFAULT_TIMEOUT = int(os.environ.get("GHIDRA_MCP_TIMEOUT", "30"))

# The only endpoints this oracle touches -- all read-only GETs.
READ_ENDPOINTS = frozenset(
    {
        "check_connection",
        "get_version",
        "get_metadata",
        "list_functions",
        "decompile_function",
        "disassemble_function",
        "get_function_by_address",
        "get_xrefs_to",
        "get_xrefs_from",
        "get_function_callers",
        "get_function_callees",
    }
)


class GhidraError(RuntimeError):
    """Connection failures, HTTP errors, and unknown/unsupported endpoints."""


class GhidraClient:
    """Fetch read-only views from a running ghidra-mcp plugin.

    Example::

        gc = GhidraClient()
        if gc.is_alive():
            print(gc.decompile("0x80042304"))
    """

    def __init__(
        self,
        base_url: str = DEFAULT_URL,
        timeout: int = DEFAULT_TIMEOUT,
        auth_token: str | None = None,
        session: Any | None = None,
    ) -> None:
        if requests is None:  # pragma: no cover
            raise GhidraError(
                "The 'requests' package is required. "
                "pip install -r tools/requirements-python.txt"
            )
        self.base_url = base_url.rstrip("/")
        self.timeout = timeout
        self.session = session or requests.Session()
        token = auth_token or os.environ.get("GHIDRA_MCP_AUTH_TOKEN")
        if token:
            self.session.headers["Authorization"] = f"Bearer {token}"

    # -- core ---------------------------------------------------------------

    def get(self, endpoint: str, **params: Any) -> Any:
        """GET a read-only endpoint. ``None`` params are dropped.

        Returns parsed JSON when the response is JSON, else the stripped text.
        """
        if endpoint not in READ_ENDPOINTS:
            raise GhidraError(
                f"'{endpoint}' is not an allowed read endpoint. "
                f"This client is read-only; allowed: {sorted(READ_ENDPOINTS)}."
            )
        clean = {k: v for k, v in params.items() if v is not None}
        url = f"{self.base_url}/{endpoint}"
        try:
            resp = self.session.request("GET", url, params=clean, timeout=self.timeout)
        except Exception as exc:  # requests.RequestException + friends
            raise GhidraError(
                f"Could not reach Ghidra at {self.base_url} ({exc}). "
                "Is the plugin's MCP server started?"
            ) from exc
        if resp.status_code != 200:
            raise GhidraError(f"HTTP {resp.status_code} from /{endpoint}: {resp.text.strip()}")
        text = resp.text.strip()
        try:
            return json.loads(text)
        except (json.JSONDecodeError, ValueError):
            return text

    # -- convenience --------------------------------------------------------

    def is_alive(self) -> bool:
        """True if the plugin answers a health check; never raises."""
        try:
            self.get("check_connection")
            return True
        except GhidraError:
            return False

    def check_connection(self) -> Any:
        return self.get("check_connection")

    def get_version(self) -> Any:
        return self.get("get_version")

    def get_metadata(self) -> Any:
        return self.get("get_metadata")

    def list_functions(self, program: str | None = None) -> Any:
        return self.get("list_functions", program=program)

    def decompile(self, address: str, program: str | None = None) -> Any:
        return self.get("decompile_function", address=address, program=program)

    def disassemble(self, address: str, program: str | None = None) -> Any:
        return self.get("disassemble_function", address=address, program=program)

    def function_at(self, address: str, program: str | None = None) -> Any:
        return self.get("get_function_by_address", address=address, program=program)

    def xrefs_to(self, address: str, program: str | None = None) -> Any:
        return self.get("get_xrefs_to", address=address, program=program)

    def xrefs_from(self, address: str, program: str | None = None) -> Any:
        return self.get("get_xrefs_from", address=address, program=program)

    def callers(self, address: str, program: str | None = None) -> Any:
        return self.get("get_function_callers", address=address, program=program)

    def callees(self, address: str, program: str | None = None) -> Any:
        return self.get("get_function_callees", address=address, program=program)


def decompiled_text(result: Any) -> str:
    """Best-effort extraction of C text from a decompile response."""
    if isinstance(result, str):
        return result
    if isinstance(result, dict):
        for key in ("decompilation", "decompiled", "code", "c", "result"):
            value = result.get(key)
            if isinstance(value, str):
                return value
    return json.dumps(result, indent=2)
