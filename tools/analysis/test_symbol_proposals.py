#!/usr/bin/env python3
"""Queue inventory, stable IDs, proposal preservation and idempotence tests."""
import hashlib
import json
from pathlib import Path
import tempfile
import unittest

import symbol_proposals as queue


class ProposalQueueTest(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name)
        self.output = self.root / 'queue'
        self.write('config/main_function_audit_by_addr.json', {'addresses': {
            '0x80041000': {'kind': 'matched_c', 'source': '/other/src/main/matched/start.c',
                           'aliases': ['start'], 'enclosed_by': '0x80041000'},
            '0x80041004': {'kind': 'covered', 'source': None,
                           'aliases': ['func_80041004'], 'enclosed_by': '0x80041000'},
            '0x80042000': {'kind': 'data', 'aliases': ['data']},
        }})
        self.write('config/level3_functions.json', {'functions': [
            {'address': '0x80041000', 'name': 'start', 'size': 16,
             'source': 'src/main/matched/start.c'}]})
        self.records = {'A:0x80132008': {
            'name': 'a_step', 'kind': 'func', 'landed': True}}
        self.write('config/symbol_names.json', {'symbols': self.records})
        self.text('config/overlay/A.sym', '0x80132000:32:a_step\n')
        self.text('config/overlay/B.sym', '0x80132000:32:func_80132008\n')

    def text(self, name, content):
        path = self.root / name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(content)

    def write(self, name, data):
        self.text(name, json.dumps(data))

    def test_scope_and_prologue_not_absorbed_head_define_identity(self):
        rows = queue.inventory(self.root)
        self.assertIn('A:0x80132008', rows)
        self.assertIn('B:0x80132008', rows)
        self.assertEqual(rows['A:0x80132008']['window_address'], '0x80132000')
        self.assertEqual(rows['A:0x80132008']['proposal']['state'], 'landed')
        self.assertEqual(rows['B:0x80132008']['proposal']['state'], 'unreviewed')
        self.assertEqual(rows['MAIN:0x80041004']['proposal']['state'], 'deferred')
        self.assertNotIn('MAIN:0x80042000', rows)
        self.assertEqual(rows['MAIN:0x80041000']['source'], 'src/main/matched/start.c')

    def test_source_directives_document_main_window_and_calls(self):
        self.text('src/main/matched/start.c',
                  '/* @SymbolStart: 0x80040FFC */\n/* @SymbolSize: 20 */\n'
                  'void start(void) { next_step(); if (flag) { draw(); } }\n')
        rows = queue.inventory(self.root)
        entry = rows['MAIN:0x80041000']
        self.assertEqual(entry['window_address'], '0x80040FFC')
        self.assertEqual(entry['window_end'], '0x80041010')
        self.assertEqual(entry['words'], 5)
        self.assertEqual(entry['call_tokens_best_effort'], ['draw', 'next_step'])
        self.assertIsNotNone(entry['source_sha256'])

    def test_refresh_preserves_proposals_and_is_idempotent(self):
        rows = queue.refresh(self.root, self.output)
        proposal = rows['B:0x80132008']['proposal']
        proposal.update(state='proposed', candidates=['b_step'],
                        evidence=['table dispatch corroborates stage role'])
        queue.write_queue(self.output, rows)
        before = {path.relative_to(self.output): path.read_bytes()
                  for path in self.output.rglob('*.json')}
        refreshed = queue.refresh(self.root, self.output)
        self.assertEqual(refreshed['B:0x80132008']['proposal'], proposal)
        after = {path.relative_to(self.output): path.read_bytes()
                 for path in self.output.rglob('*.json')}
        self.assertEqual(before, after)

    def test_registry_landing_updates_claimed_record_by_stable_id(self):
        rows = queue.refresh(self.root, self.output)
        rows['B:0x80132008']['proposal'].update(state='claimed', candidates=['b_step'])
        queue.write_queue(self.output, rows)
        self.records['B:0x80132008'] = {'name': 'b_step', 'kind': 'func', 'landed': True}
        self.write('config/symbol_names.json', {'symbols': self.records})
        self.text('config/overlay/B.sym', '0x80132000:32:b_step\n')
        refreshed = queue.refresh(self.root, self.output)
        self.assertEqual(refreshed['B:0x80132008']['proposal']['state'], 'landed')
        self.assertEqual(refreshed['B:0x80132008']['proposal']['candidates'], ['b_step'])

    def test_external_landing_of_unreviewed_record_is_discovered(self):
        queue.refresh(self.root, self.output)
        self.records['B:0x80132008'] = {'name': 'b_step', 'kind': 'func', 'landed': True}
        self.write('config/symbol_names.json', {'symbols': self.records})
        self.text('config/overlay/B.sym', '0x80132000:32:b_step\n')
        refreshed = queue.refresh(self.root, self.output)
        self.assertEqual(refreshed['B:0x80132008']['proposal']['state'], 'landed')

    def test_removed_entries_are_retained_as_stale_not_deleted(self):
        queue.refresh(self.root, self.output)
        self.text('config/overlay/B.sym', '')
        refreshed = queue.refresh(self.root, self.output)
        self.assertEqual(refreshed['B:0x80132008']['proposal']['state'], 'stale')

    def test_import_is_unapproved_and_refuses_stale_source(self):
        rows = queue.inventory(self.root)
        source = rows['B:0x80132008']['source']
        self.text(source, 'func_80132008:\n  jr ra\n')
        self.write('batch.json', {'entries': [{
            'id': 'B:0x80132008', 'source': source,
            'current_name': 'func_80132008',
            'source_sha256': hashlib.sha256((self.root / source).read_bytes()).hexdigest(),
        }]})
        self.write('results.json', {'proposals': [{
            'id': 'B:0x80132008', 'state': 'proposed', 'candidates': ['b_step'],
            'evidence': ['B assembly/caller table establishes stage role'], 'uncertainty': [],
        }]})
        missing = queue.import_proposals(self.root, rows, self.root / 'batch.json',
                                         self.root / 'results.json', 'test-proposer')
        self.assertEqual(missing, [])
        self.assertEqual(rows['B:0x80132008']['proposal']['state'], 'proposed')
        self.assertIsNone(rows['B:0x80132008']['proposal']['reviewed_by'])
        self.text(source, 'changed')
        with self.assertRaisesRegex(ValueError, 'stale source'):
            queue.import_proposals(self.root, rows, self.root / 'batch.json',
                                   self.root / 'results.json', 'test-proposer')

    def test_each_shard_stays_small_and_unknown_states_are_rejected(self):
        rows = queue.inventory(self.root)
        template = rows['B:0x80132008']
        for index in range(150):
            identity = f'B:0x{0x80133000 + index * 4:08X}'
            rows[identity] = dict(template, id=identity)
        queue.write_queue(self.output, rows)
        for path in self.output.rglob('*.json'):
            self.assertLess(len(path.read_text().splitlines()), 600)
        rows['A:0x80132008']['proposal']['state'] = 'magically_verified'
        with self.assertRaisesRegex(ValueError, 'unknown state'):
            queue.write_queue(self.output, rows)


if __name__ == '__main__':
    unittest.main()
