"""Lean, read-only Ghidra comprehension oracle for toki1-decomp.

Talks straight to a running ghidra-mcp plugin's HTTP server (127.0.0.1:8089)
to fetch decompilation / disassembly / xrefs so you can understand a function.
It does NOT produce byte-perfect C -- m2c / decomp-permuter / ido_diff own that.
See tools/ghidra/README.md.
"""

from .client import GhidraClient, GhidraError, decompiled_text

__all__ = ["GhidraClient", "GhidraError", "decompiled_text"]
