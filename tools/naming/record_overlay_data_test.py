"""Tests for record_overlay_data: the overlay-DATA record writer.

Every test runs the real tool as a subprocess against a throw-away git
repository under a temp dir (--ledger redirects the ledger, and with it the
tree root and landing.lock), so the fences, the record schema, the pending
update path and the idempotency rule are exercised end to end and the real
config/symbol_names.json is never touched.

    python3 tools/naming/record_overlay_data_test.py
"""
import json
import os
import subprocess
import sys
import tempfile
import unittest

TOOL = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'record_overlay_data.py')
PY = sys.executable

C_DATE = '''/* date overlay data unit */
extern u16 D_8015BDA4;
int date_helper(void)
{
    return D_8015BDA4;
}
'''
C_COMMENT = '/* the old_working_copy name lives only in this comment */\nvoid cmt(void) {}\n'


def write(root, path, text):
    p = os.path.join(root, path)
    os.makedirs(os.path.dirname(p), exist_ok=True)
    with open(p, 'w') as fh:
        fh.write(text)


def git(root, *a):
    subprocess.run(['git'] + list(a), cwd=root, check=True, capture_output=True)


def row(**kw):
    r = {'id': 'DATE:0x8015BDA4', 'decision': 'LAND', 'name': 'date_meter',
         'size': 8, 'type': 'u16', 'tier': 'T4', 'style': 'derived',
         'confidence': 'DERIVED', 'evidence': 'reads/writes in date_main'}
    r.update(kw)
    return r


def seed_ledger():
    def rec(name, landed):
        return {'name': name, 'kind': 'data', 'scope': 'DATE', 'size': 4,
                'tier': 'T4', 'style': 'derived', 'confidence': 'DERIVED',
                'evidence': 'seed record', 'proposed_by': 't', 'date': '2026-10-18',
                'landed': landed}
    return {'version': 1, 'generated_by': 'test', 'description': 'fixture',
            'symbols': {'DATE:0x8015FFFF': rec('other_rec', False),
                        'DATE:0x8015AAAA': rec('landed_one', True),
                        'DATE:0x8015BBBB': rec('pending_one', False)},
            'hints': {}}


class Fixture(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = self.tmp.name
        write(self.root, 'config/symbol_names.json', json.dumps(seed_ledger(), indent=1) + '\n')
        write(self.root, 'src/overlay/DATE/func_8015BDA4.c', C_DATE)
        write(self.root, 'src/overlay/DATE/notes.c', C_COMMENT)
        os.makedirs(os.path.join(self.root, 'src/overlay/EVENT'))
        git(self.root, 'init', '-q')
        git(self.root, 'config', 'user.email', 't@t')
        git(self.root, 'config', 'user.name', 't')
        git(self.root, 'add', '-A')
        git(self.root, 'commit', '-q', '-m', 'fixture')
        self.ledger = os.path.join(self.root, 'config', 'symbol_names.json')

    def tearDown(self):
        self.tmp.cleanup()

    def run_tool(self, rows, *extra):
        path = os.path.join(self.root, 'decided.json')
        with open(path, 'w') as fh:
            json.dump(rows, fh)
        p = subprocess.run([PY, TOOL, path, '--ledger', self.ledger] + list(extra),
                           capture_output=True, text=True)
        return p.returncode, p.stdout + p.stderr

    def read(self):
        with open(self.ledger) as fh:
            return json.load(fh)['symbols']

    def bytes(self):
        with open(self.ledger, 'rb') as fh:
            return fh.read()


class LandRowTests(Fixture):
    def test_land_row_writes_pending_record(self):
        rc, out = self.run_tool([row()], '--apply')
        self.assertEqual(rc, 0, out)
        e = self.read()['DATE:0x8015BDA4']
        self.assertEqual(e['name'], 'date_meter')
        self.assertEqual(e['kind'], 'data')
        self.assertEqual(e['scope'], 'DATE')
        self.assertEqual(e['size'], 8)
        self.assertEqual(e['type'], 'u16')
        self.assertEqual(e['tier'], 'T4')
        self.assertEqual(e['style'], 'derived')
        self.assertEqual(e['confidence'], 'DERIVED')
        self.assertIn('reads/writes in date_main', e['evidence'])
        self.assertIn('reviewed:consistent', e['evidence'])
        self.assertEqual(e['proposed_by'], 'overlay-data blind-review')
        self.assertIs(e['landed'], False)

    def test_default_is_dry_run(self):
        before = self.bytes()
        rc, out = self.run_tool([row()])
        self.assertEqual(rc, 0, out)
        self.assertIn('dry run', out)
        self.assertEqual(self.bytes(), before)

    def test_bare_id_gets_ov_scope(self):
        rc, out = self.run_tool([row(id='0x8015BDA4')], '--ov', 'DATE', '--apply')
        self.assertEqual(rc, 0, out)
        self.assertEqual(self.read()['DATE:0x8015BDA4']['name'], 'date_meter')

    def test_note_lands_in_evidence(self):
        rc, out = self.run_tool([row(note='u16 [4] array')], '--apply')
        self.assertEqual(rc, 0, out)
        self.assertIn('note: u16 [4] array', self.read()['DATE:0x8015BDA4']['evidence'])


class SkipTests(Fixture):
    def test_defer_row_is_not_recorded(self):
        rc, out = self.run_tool([row(decision='DEFER')], '--apply')
        self.assertEqual(rc, 0, out)
        self.assertIn('SKIP', out)
        self.assertNotIn('DATE:0x8015BDA4', self.read())

    def test_row_for_another_overlay_is_skipped(self):
        rc, out = self.run_tool([row()], '--ov', 'TACO', '--apply')
        self.assertEqual(rc, 0, out)
        self.assertIn('row for another overlay', out)
        self.assertNotIn('DATE:0x8015BDA4', self.read())


class RefusalTests(Fixture):
    def assert_refused(self, out, needle, key='DATE:0x8015BDA4'):
        self.assertIn('REFUSE', out)
        self.assertIn(key, out)
        self.assertIn(needle, out)
        self.assertNotIn(key, self.read())

    def test_collision_with_existing_record_refused(self):
        rc, out = self.run_tool([row(name='other_rec')], '--apply')
        self.assertEqual(rc, 1, out)
        self.assert_refused(out, 'already held by DATE:0x8015FFFF')

    def test_collision_with_tree_identifier_refused(self):
        rc, out = self.run_tool([row(name='date_helper')], '--apply')
        self.assertEqual(rc, 1, out)
        self.assert_refused(out, 'already used as an identifier')

    def test_comment_mention_is_not_a_collision(self):
        rc, out = self.run_tool([row(name='old_working_copy')], '--apply')
        self.assertEqual(rc, 0, out)
        self.assertEqual(self.read()['DATE:0x8015BDA4']['name'], 'old_working_copy')

    def test_landed_overwrite_refused(self):
        before = self.read()['DATE:0x8015AAAA']
        rc, out = self.run_tool([row(id='DATE:0x8015AAAA', name='new_want')], '--apply')
        self.assertEqual(rc, 1, out)
        self.assertIn('REFUSE DATE:0x8015AAAA', out)
        self.assertIn('never overwrites', out)
        self.assertEqual(self.read()['DATE:0x8015AAAA'], before)

    def test_address_shaped_name_refused(self):
        for name in ('D_8015BDA4', 'func_8015Bda4'):
            with self.subTest(name=name):
                rc, out = self.run_tool([row(name=name)], '--apply')
                self.assertEqual(rc, 1, out)
                self.assert_refused(out, 'address-shaped')

    def test_sony_name_without_corroboration_refused(self):
        for r in (row(name='GsWorkAreaBase'),
                  row(name='date_meter', style='sony', tier='T2'),
                  row(name='date_meter', tier='T3', evidence='o.bin-shaped claim, no marker')):
            with self.subTest(row=r):
                rc, out = self.run_tool([r], '--apply')
                self.assertEqual(rc, 1, out)
                self.assert_refused(out, 'corroborated:')

    def test_sony_name_with_corroboration_ok(self):
        rc, out = self.run_tool(
            [row(name='GsWorkAreaBase', style='sony', tier='T2', confidence='HIGH',
                 evidence='verbatim GsWorkAreaBase export in Psy-Q 2.5 libgs; '
                          'corroborated: psyq archive symbol table')], '--apply')
        self.assertEqual(rc, 0, out)
        self.assertIn('corroborated: psyq archive symbol table',
                      self.read()['DATE:0x8015BDA4']['evidence'])

    def test_bad_scope_refused(self):
        for key, needle in (('EVENT:0x8015BDA4', 'never loaded'),
                            ('NOPE:0x8015BDA4', 'no src/overlay/NOPE/'),
                            ('0x800E8C84', 'main-scope')):
            with self.subTest(key=key):
                rc, out = self.run_tool([row(id=key)], '--apply')
                self.assertEqual(rc, 1, out)
                self.assert_refused(out, needle, key=key)


class UpdateTests(Fixture):
    def test_pending_update_preserves_history(self):
        rc, out = self.run_tool([row(id='DATE:0x8015BBBB', name='pending_two', size=8)], '--apply')
        self.assertEqual(rc, 0, out)
        e = self.read()['DATE:0x8015BBBB']
        self.assertEqual(e['name'], 'pending_two')
        self.assertEqual(len(e['history']), 1)
        old = e['history'][0]
        self.assertEqual(old['name'], 'pending_one')
        self.assertEqual(old['confidence'], 'REVERTED')
        self.assertEqual(old['size'], 4)
        self.assertIs(old['landed'], False)

    def test_idempotent_rerun(self):
        rc, out = self.run_tool([row()], '--apply')
        self.assertEqual(rc, 0, out)
        self.assertIn('1 records added', out)
        first = self.bytes()
        rc, out = self.run_tool([row()], '--apply')
        self.assertEqual(rc, 0, out)
        self.assertIn('0 records added, 0 updated', out)
        self.assertIn('already recorded', out)
        self.assertEqual(self.bytes(), first)


if __name__ == '__main__':
    unittest.main()
