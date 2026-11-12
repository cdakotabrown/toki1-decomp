#!/usr/bin/env python3
"""Regression coverage for scoped PC-status keys, including legacy duplicates."""
import json
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parent))
import apply_overlay_symbols as lander


class ScopedStatusTest(unittest.TestCase):
    pairs = {'NAME_ENT': {'func_8013425C': 'name_ent_glyph_write'}}

    def test_single_and_double_colon_tokens_are_scoped(self):
        before = {
            'NAME_ENT:func_8013425C': {
                'overlay': 'NAME_ENT',
                'ps1_source': 'src/overlay/NAME_ENT/func_8013425C.c',
            },
            'NAME_ENT::func_8013425C': 'NAME_ENT/func_8013425C',
            'OTHER:func_8013425C': 'src/overlay/OTHER/func_8013425C.c',
            'NAME_ENT:func_8013425C_extra': 'unchanged',
            'XNAME_ENT:func_8013425C': 'unchanged',
        }
        result = lander.Walker(self.pairs).walk(before, None)
        self.assertEqual(result['NAME_ENT:name_ent_glyph_write']['ps1_source'],
                         'src/overlay/NAME_ENT/name_ent_glyph_write.c')
        self.assertEqual(result['NAME_ENT::name_ent_glyph_write'],
                         'NAME_ENT/name_ent_glyph_write')
        for key in ('OTHER:func_8013425C', 'NAME_ENT:func_8013425C_extra',
                    'XNAME_ENT:func_8013425C'):
            self.assertEqual(result[key], before[key])

    def test_duplicate_json_fallback_preserves_keys_and_scope(self):
        before = '''{
  "functions": {
    "CD_readm": {},
    "CD_readm": {},
    "NAME_ENT:func_8013425C": {
      "category": "B",
      "ps1_source": "src/overlay/NAME_ENT/func_8013425C.c"
    },
    "func_8013425C": {
      "category": "A",
      "ps1_source": "src/overlay/NAME_ENT/func_8013425C.c"
    },
    "OTHER:func_8013425C": {
      "category": "B",
      "ps1_source": "src/overlay/OTHER/func_8013425C.c"
    },
    "NAME_ENT:func_8013425C": {
      "category": "B",
      "ps1_source": "src/overlay/OTHER/func_8013425C.c"
    }
  }
}
'''
        with self.assertRaisesRegex(ValueError, 'duplicate key CD_readm'):
            json.loads(before, object_pairs_hook=lander.dup_pairs_hook)
        result = lander.special_json('config/pc_port_function_status.json',
                                     before, self.pairs)
        expected = before.replace(
            '"NAME_ENT:func_8013425C": {\n      "category": "B",\n'
            '      "ps1_source": "src/overlay/NAME_ENT/func_8013425C.c"',
            '"NAME_ENT:name_ent_glyph_write": {\n      "category": "B",\n'
            '      "ps1_source": "src/overlay/NAME_ENT/name_ent_glyph_write.c"')
        expected = expected.replace(
            '"func_8013425C": {\n      "category": "A",\n'
            '      "ps1_source": "src/overlay/NAME_ENT/func_8013425C.c"',
            '"name_ent_glyph_write": {\n      "category": "A",\n'
            '      "ps1_source": "src/overlay/NAME_ENT/name_ent_glyph_write.c"')
        self.assertEqual(result, expected)
        self.assertIsNone(lander.special_json(
            'config/pc_port_function_status.json', result, self.pairs))
        with tempfile.TemporaryDirectory() as temporary:
            path = Path(temporary) / 'status.json'
            path.write_text(json.dumps({'NAME_ENT:func_8013425C': 'old'}, indent=2))
            self.assertIn('NAME_ENT:name_ent_glyph_write',
                          lander.rewrite_config(str(path), self.pairs))

    def test_legacy_suffix_requires_source_scope(self):
        record = {'overlay': 'NAME_ENT',
                  'ps1_source': 'src/overlay/NAME_ENT/func_8013425C.c',
                  'func_name': 'func_8013425C'}
        before = {'func_8013425C_NAME_ENT': record,
                  'func_8013425C_OTHER': dict(record)}
        result = lander.Walker(self.pairs).walk(before, None)
        renamed = result['name_ent_glyph_write_NAME_ENT']
        self.assertEqual(renamed['func_name'], 'name_ent_glyph_write')
        self.assertEqual(renamed['ps1_source'],
                         'src/overlay/NAME_ENT/name_ent_glyph_write.c')
        self.assertIn('func_8013425C_OTHER', result)
        wrong_source = {'func_8013425C_NAME_ENT': {
            'ps1_source': 'src/overlay/OTHER/func_8013425C.c'}}
        self.assertEqual(lander.Walker(self.pairs).walk(wrong_source, None),
                         wrong_source)

    def test_duplicate_legacy_suffix_and_slash_update_name_fields(self):
        for key in ('func_8013425C_NAME_ENT', 'NAME_ENT/func_8013425C'):
            record = {'category': 'C',
                      'ps1_source': 'src/overlay/NAME_ENT/func_8013425C.c',
                      'overlay': 'NAME_ENT', 'func_name': 'func_8013425C'}
            before = json.dumps({'functions': {key: record}}, indent=2) + '\n'
            result = lander.special_json('config/pc_port_function_status.json',
                                         before, self.pairs)
            self.assertEqual(result,
                             before.replace('func_8013425C', 'name_ent_glyph_write'))
            self.assertIsNone(lander.special_json(
                'config/pc_port_function_status.json', result, self.pairs))
            wrong = before.replace('src/overlay/NAME_ENT/', 'src/overlay/OTHER/')
            self.assertIsNone(lander.special_json(
                'config/pc_port_function_status.json', wrong, self.pairs))

    def test_partially_renamed_record_finishes_without_churn(self):
        before = '''{
  "name_ent_glyph_write_NAME_ENT": {
    "category": "C",
    "ps1_source": "src/overlay/NAME_ENT/name_ent_glyph_write.c",
    "func_name": "func_8013425C"
  }
}
'''
        result = lander.special_json('config/pc_port_function_status.json',
                                     before, self.pairs)
        self.assertEqual(result, before.replace('"func_8013425C"',
                                                '"name_ent_glyph_write"'))
        self.assertIsNone(lander.special_json(
            'config/pc_port_function_status.json', result, self.pairs))


if __name__ == '__main__':
    unittest.main()
