"""Tests for symbol-tier gating in apply_debug_symbols."""
from types import SimpleNamespace
import unittest
from unittest.mock import patch

import apply_debug_symbols


class DerivedNamePlanTests(unittest.TestCase):
    address = "0x80012345"
    registry_names = {
        "names": {},
        "by_addr": {0x80012345: [{"name": "func_80012345"}]},
    }

    def plan(self, *, tier="T4", style="derived", confidence="DERIVED",
             evidence="reviewed:consistent independent readers agree",
             tiers=None):
        entry = {
            "name": "derived_fixture_symbol",
            "kind": "func",
            "size": 32,
            "tier": tier,
            "style": style,
            "confidence": confidence,
            "evidence": evidence,
            "landed": False,
        }
        args = SimpleNamespace(
            tiers=tiers or {"HIGH", "MEDIUM", "DERIVED"},
            require_review=True,
            allow_decl=[],
            allow_locked=[],
            exclude=[],
        )
        with patch.object(apply_debug_symbols, "refs", return_value=[]), \
             patch.object(apply_debug_symbols, "file_moves", return_value=[]), \
             patch.object(apply_debug_symbols, "is_excluded", return_value=False), \
             patch.object(apply_debug_symbols, "is_locked", return_value=False):
            return apply_debug_symbols.plan(
                entry, self.address, self.registry_names, set(), args
            )

    def test_reviewed_t4_candidate_passes_explicit_derived_gate(self):
        ok, reasons, *_ = self.plan()
        self.assertTrue(ok, reasons)

    def test_derived_candidate_needs_derived_minimum(self):
        ok, reasons, *_ = self.plan(tiers={"HIGH", "MEDIUM"})
        self.assertFalse(ok)
        self.assertIn("confidence DERIVED below --min-tier", reasons)

    def test_derived_confidence_requires_t4_derived_style(self):
        ok, reasons, *_ = self.plan(tier="T3", style="obin")
        self.assertFalse(ok)
        self.assertIn("DERIVED confidence requires a T4 derived-style candidate", reasons)

    def test_explicit_derived_gate_does_not_skip_review(self):
        ok, reasons, *_ = self.plan(evidence="body evidence only")
        self.assertFalse(ok)
        self.assertIn("no `reviewed:consistent` verdict (symbol_names.py review)", reasons)


if __name__ == "__main__":
    unittest.main()
