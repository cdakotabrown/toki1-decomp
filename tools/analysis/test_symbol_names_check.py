"""Check registered function windows against prologue-keyed naming records."""
import contextlib
import io
import json
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import symbol_names
import naming_lint


class CheckWindowTests(unittest.TestCase):
    def check(self, named_addr, registry_addr, category="head", true_start=None):
        entry = {"name": "test_init", "kind": "func", "landed": True}
        names = {named_addr: entry}
        registry = {"functions": [{"name": "test_init", "address": registry_addr}]}
        boundary = {named_addr: {"category": category,
                                 "true_start": true_start or registry_addr}}
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "boundaries.json"
            path.write_text(json.dumps({"boundaries": boundary}))
            output = io.StringIO()
            with patch.object(symbol_names, "BOUNDARIES", str(path)), \
                 patch.object(symbol_names, "load_names", return_value={"symbols": names}), \
                 patch.object(symbol_names, "load_registry", return_value=registry), \
                 contextlib.redirect_stdout(output):
                result = symbol_names.cmd_check(None)
            return result, output.getvalue()

    def test_absorbed_head(self):
        self.assertEqual(self.check("0x80068030", "0x8006801C")[0], 0)

    def test_unabsorbed_head(self):
        self.assertEqual(self.check("0x80068030", "0x80068030",
                                    true_start="0x8006801C")[0], 0)

    def test_wrong_address_is_still_rejected(self):
        result, output = self.check("0x80068030", "0x80068018",
                                    true_start="0x8006801C")
        self.assertEqual(result, 1)
        self.assertIn("REGISTRY DISAGREES", output)

    def test_non_head_cannot_move(self):
        self.assertEqual(self.check("0x80068030", "0x8006801C",
                                    category="phantom")[0], 1)

    def test_reverted_proximity_name_is_not_a_collision(self):
        index = naming_lint.Index()
        self.assertNotIn(0x80086CD0, index.main.get("holiday_tel", set()))
        self.assertIn(0x80086CD0, index.main["MDEC_out"])


if __name__ == "__main__":
    unittest.main()
