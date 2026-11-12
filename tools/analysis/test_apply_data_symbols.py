"""Tests for apply_data_symbols: the data-record lander.

Every test runs the real tool against a throw-away git repository (fixture
files under a temp dir, symbol_names/retail/registry paths redirected there),
so the file scan, the scope rules, the collision rules, the locked-file rule,
the ledgers and the revert are exercised end to end.  The naming linter and
the overlay resolver are stubbed: they have their own gates.

    python3 tools/analysis/test_apply_data_symbols.py
"""
import json
import os
import subprocess
import sys
import tempfile
import unittest
from types import SimpleNamespace
from unittest.mock import patch

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import apply_data_symbols as A  # noqa: E402
import symbol_names as SN  # noqa: E402

SONY = {'now_example', 'GsORGOFSX', 'GsOUT_PACKET_P'}
EV_OK = ('T2 (Sony source shape): libgs; corroborated: shape match against Sony\'s own code; '
         'reviewed:consistent (2026-10-08, reviewer): stores in Sony order')

C_MAIN = '''/* the example object, and its volatile view D_800EF123_v below */
extern s16 D_800EF123;
extern volatile s16 D_800EF123_v;
extern u8 D_800EF124;
void foo(void)
{
    D_800EF123 = 0;
    D_800ef123 = D_800EF123_v + D_800EF124;
}
'''


def write(root, path, text):
    p = os.path.join(root, path)
    os.makedirs(os.path.dirname(p), exist_ok=True)
    open(p, 'w').write(text)


def git(root, *a):
    subprocess.run(['git'] + list(a), cwd=root, check=True, capture_output=True)


class Fixture(unittest.TestCase):
    name = 'now_example'
    addr = '0x800EF123'

    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = self.tmp.name
        r = self.root
        write(r, 'src/main/matched/foo.c', C_MAIN)
        write(r, 'src/main/matched/bar.c', 'extern s16 D_800ef123;\nvoid bar(void) { D_800EF123 = 1; }\n')
        write(r, 'src/overlay/ABC/func_80132000.c', 'extern s16 D_800EF123;\nvoid func_80132000(void) { D_800EF123 = 2; }\n')
        write(r, 'include/overlay/globals.h', 'extern s16 D_800EF123;\n')
        write(r, 'pc/src/globals.c', 's16 D_800EF123 = 0;\n')
        write(r, 'pc/3rd/vendor/x.c', 'int D_800EF123;\n')
        write(r, 'docs/notes.md', 'D_800EF123 is the example\n')
        write(r, 'tools/maspsx/tests/t.py', 'x = "D_800EF123"\n')
        write(r, 'tools/ido/post.py', 'SYM = "D_800EF123"\n')
        write(r, 'asm/main/matched/foo.s', '# D_800EF123 in a comment\n')
        write(r, 'config/main_retail_symbols.json', '{}')
        write(r, 'config/level3_functions.json', json.dumps({'functions': [
            {'name': 'foo', 'address': '0x80050000', 'size': 8, 'source': 'src/main/matched/foo.c'}]}))
        self.names = {'version': 1, 'symbols': {
            self.addr: {'name': self.name, 'kind': 'data', 'scope': 'main', 'size': 8,
                        'tier': 'T2', 'style': 'sony', 'confidence': 'HIGH',
                        'evidence': EV_OK, 'landed': False},
            '0x800EF200': {'name': 'other_one', 'kind': 'data', 'scope': 'main', 'size': 4,
                           'tier': 'T4', 'style': 'derived', 'confidence': 'DERIVED',
                           'evidence': 'reviewed:consistent', 'landed': False}},
            'hints': {}}
        write(r, 'config/symbol_names.json', json.dumps(self.names, indent=1) + '\n')
        git(r, 'init', '-q')
        git(r, 'config', 'user.email', 't@t')
        git(r, 'config', 'user.name', 't')
        git(r, 'add', '-A')
        git(r, 'commit', '-q', '-m', 'fixture')
        self.patches = [
            patch.object(SN, 'ROOT', r),
            patch.object(SN, 'NAMES', os.path.join(r, 'config/symbol_names.json')),
            patch.object(SN, 'RETAIL', os.path.join(r, 'config/main_retail_symbols.json')),
            patch.object(SN, 'REGISTRY', os.path.join(r, 'config/level3_functions.json')),
            patch.object(A, 'lint_findings', lambda n, a: ([], n in SONY)),
            patch.object(A, '_TOKENS', None),
            patch.object(A, '_CENSUS', {'objects': {}, 'addresses': {}}),
        ]
        for p in self.patches:
            p.start()

    def tearDown(self):
        for p in self.patches:
            p.stop()
        self.tmp.cleanup()

    def args(self, **kw):
        d = dict(dry_run=False, apply=True, tiers={'HIGH'}, require_review=True, only=[self.addr],
                 batch=10, lock=[], allow_locked=[], allow_decl=[], allow_unreferenced=False,
                 no_resolver_check=True, revert=False, refuted=False, search=False, no_git_lock=False)
        d.update(kw)
        return SimpleNamespace(**d)

    def text(self, path):
        return open(os.path.join(self.root, path)).read()

    def record(self, addr=None):
        return json.load(open(os.path.join(self.root, 'config/symbol_names.json')))['symbols'][addr or self.addr]

    def plan(self, **kw):
        doc = SN.load_names()
        reg = SN.load_registry()
        fn = {f['name']: int(f['address'], 16) for f in reg['functions']}
        fa = {int(f['address'], 16): f['name'] for f in reg['functions']}
        args = self.args(**kw)
        e = doc['symbols'][args.only[0]]
        return A.plan(args.only[0], e, fa, fn, A.locked_files([]), args)

    def run_quiet(self, fn, args):
        import io
        from contextlib import redirect_stdout
        buf = io.StringIO()
        with redirect_stdout(buf):
            fn(args)
        return buf.getvalue()


class LandTests(Fixture):
    def test_exact_token_renamed_in_scope_only(self):
        out = self.run_quiet(A.landed_main, self.args())
        self.assertIn('LAND', out)
        foo = self.text('src/main/matched/foo.c')
        # both hex cases renamed, types intact, comment untouched
        self.assertIn('extern s16 now_example;', foo)
        self.assertIn('now_example = 0;', foo)
        self.assertNotIn('D_800EF123;', foo)
        self.assertNotIn('D_800ef123', foo)
        # the address-identical alias view and the interior token are NOT renamed
        self.assertIn('extern volatile s16 D_800EF123_v;', foo)
        self.assertIn('extern u8 D_800EF124;', foo)
        self.assertIn('D_800EF123_v + D_800EF124', foo)
        # overlays (main data is global), headers, the PC port, the build scripts
        for p in ('src/overlay/ABC/func_80132000.c', 'include/overlay/globals.h', 'pc/src/globals.c',
                  'src/main/matched/bar.c'):
            self.assertIn('now_example', self.text(p), p)
            self.assertNotIn('D_800EF123', self.text(p).replace('D_800EF123_v', ''), p)
        self.assertEqual(self.text('tools/ido/post.py'), 'SYM = "now_example"\n')
        # vendor, prose, test fixtures and asm comments are left alone
        self.assertIn('D_800EF123', self.text('pc/3rd/vendor/x.c'))
        self.assertIn('D_800EF123', self.text('docs/notes.md'))
        self.assertIn('D_800EF123', self.text('tools/maspsx/tests/t.py'))
        self.assertIn('D_800EF123', self.text('asm/main/matched/foo.s').replace('now_example', 'D_800EF123'))

    def test_ledgers(self):
        self.run_quiet(A.landed_main, self.args())
        e = self.record()
        self.assertTrue(e['landed'])
        self.assertIn('src/overlay/ABC/func_80132000.c', e['landed_files'])
        self.assertNotIn('pc/3rd/vendor/x.c', e['landed_files'])
        self.assertIn('landed', e['evidence'])
        retail = json.load(open(os.path.join(self.root, 'config/main_retail_symbols.json')))
        self.assertEqual(retail['now_example']['address'], '0x800ef123')
        self.assertFalse(self.record('0x800EF200')['landed'])

    def test_dry_run_changes_nothing(self):
        before = self.text('src/main/matched/foo.c')
        out = self.run_quiet(A.landed_main, self.args(apply=False, dry_run=True))
        self.assertIn('WOULD', out)
        self.assertEqual(before, self.text('src/main/matched/foo.c'))
        self.assertFalse(self.record()['landed'])

    def test_interior_listed_not_renamed(self):
        out = self.run_quiet(A.landed_main, self.args(apply=False, dry_run=True))
        self.assertIn('D_800EF124', out)
        self.assertIn('NOT renamed', out)
        self.assertIn('alias views NOT renamed', out)
        self.assertIn('D_800EF123_v', out)

    def test_interior_field_names(self):
        self.names['symbols'][self.addr]['fields'] = {'0x1': 'second'}
        write(self.root, 'config/symbol_names.json', json.dumps(self.names, indent=1))
        out = self.run_quiet(A.landed_main, self.args(apply=False, dry_run=True))
        self.assertIn('field second', out)

    def test_bad_fields_refused(self):
        self.names['symbols'][self.addr]['fields'] = {'0x40': 'beyond', 'zz': 'bad'}
        write(self.root, 'config/symbol_names.json', json.dumps(self.names, indent=1))
        p = self.plan()
        self.assertFalse(p['ok'])
        self.assertTrue(any('beyond the object extent' in r for r in p['reasons']))
        self.assertTrue(any('not hex' in r for r in p['reasons']))


class RefusalTests(Fixture):
    def reasons(self, **kw):
        p = self.plan(**kw)
        return p['ok'], ' | '.join(p['reasons'])

    def set_record(self, **kw):
        self.names['symbols'][self.addr].update(kw)
        write(self.root, 'config/symbol_names.json', json.dumps(self.names, indent=1))

    def test_clean_record_passes(self):
        ok, why = self.reasons()
        self.assertTrue(ok, why)

    def test_needs_review(self):
        self.set_record(evidence=EV_OK.replace('reviewed:consistent', 'unreviewed'))
        ok, why = self.reasons()
        self.assertFalse(ok)
        self.assertIn('reviewed:consistent', why)

    def test_contradicted(self):
        self.set_record(evidence=EV_OK + '; reviewed:contradicted (x)')
        self.assertIn('CONTRADICTED', self.reasons()[1])

    def test_sony_needs_corroboration(self):
        self.set_record(evidence='reviewed:consistent only')
        self.assertIn('corroborated', self.reasons()[1])

    def test_sony_needs_t1_t2(self):
        self.set_record(tier='T3')
        self.assertIn('T1/T2', self.reasons()[1])

    def test_sony_style_unknown_name_refused(self):
        self.set_record(name='GsNotASonyName')
        self.assertIn('no Psy-Q archive', self.reasons()[1])

    def test_obin_name_needs_corroboration(self):
        self.set_record(name='plain_name', tier='T3', style='obin', evidence='reviewed:consistent x')
        self.assertIn('o.bin-sourced', self.reasons()[1])

    def test_derived_needs_min_tier(self):
        p = self.plan(only=['0x800EF200'])
        self.assertIn('confidence DERIVED below --min-tier', ' | '.join(p['reasons']))

    def test_address_shaped_name(self):
        self.set_record(name='D_800EF999', tier='T4', style='derived')
        self.assertIn('address-shaped', self.reasons()[1])

    def test_scope_and_range(self):
        self.set_record(scope='ABC')
        self.assertIn('overlay-local', self.reasons()[1])

    def test_registered_function_address(self):
        write(self.root, 'config/level3_functions.json', json.dumps({'functions': [
            {'name': 'foo', 'address': '0x800EF123', 'size': 8, 'source': 'src/main/matched/foo.c'}]}))
        self.assertIn('registered function', self.reasons()[1])

    def test_function_name_collision(self):
        write(self.root, 'config/level3_functions.json', json.dumps({'functions': [
            {'name': 'now_example', 'address': '0x80050000', 'size': 8, 'source': 'src/main/matched/foo.c'}]}))
        self.assertIn('registered function', self.reasons()[1])

    def test_no_references(self):
        git(self.root, 'rm', '-q', '-r', 'src', 'include', 'pc/src', 'tools/ido', 'asm')
        ok, why = self.reasons()
        self.assertFalse(ok)
        self.assertIn('no reference', why)
        ok, _ = self.reasons(allow_unreferenced=True)
        self.assertTrue(ok)

    def test_file_scope_collision_elsewhere(self):
        write(self.root, 'src/main/matched/zzz.c', 'int now_example;\n')
        ok, why = self.reasons()
        self.assertFalse(ok)
        self.assertIn('src/main/matched/zzz.c', why)

    def test_local_in_untouched_file_is_fine(self):
        write(self.root, 'src/main/matched/zzz.c', 'void z(void)\n{\n    int now_example;\n}\n')
        ok, why = self.reasons()
        self.assertTrue(ok, why)

    def test_collision_in_touched_file_is_shadowing(self):
        write(self.root, 'src/main/matched/bar.c',
              'extern s16 D_800ef123;\nvoid bar(void) { int now_example; D_800EF123 = 1; }\n')
        git(self.root, 'commit', '-q', '-am', 'local with the same name')
        ok, why = self.reasons()
        self.assertFalse(ok)
        self.assertIn('shadowing', why)
        ok, _ = self.reasons(allow_decl=['src/main/matched/bar.c'])
        self.assertTrue(ok)

    def test_comment_only_mentions_do_not_collide(self):
        write(self.root, 'src/main/matched/zzz.c', '/* now_example */\n// now_example\n')
        ok, why = self.reasons()
        self.assertTrue(ok, why)

    def test_header_collision_needs_inclusion(self):
        write(self.root, 'include/psyq/libgs.h', 'extern int now_example;\n')
        ok, why = self.reasons()
        self.assertTrue(ok, why)      # nothing includes it: a warning, not a refusal
        self.assertTrue(any('libgs.h' in w for w in self.plan()['warnings']))
        write(self.root, 'src/main/matched/foo.c', '#include "psyq/libgs.h"\n' + C_MAIN)
        ok, why = self.reasons()
        self.assertFalse(ok)
        self.assertIn('include/psyq/libgs.h', why)

    def test_locked_file(self):
        write(self.root, 'src/main/matched/bar.c', 'extern s16 D_800ef123;\n/* edited */\n')
        ok, why = self.reasons()
        self.assertFalse(ok)
        self.assertIn('locked', why)
        ok, _ = self.reasons(allow_locked=['src/main/matched/bar.c'])
        self.assertTrue(ok)

    def test_lint_error_refuses(self):
        with patch.object(A, 'lint_findings', lambda n, a: ([('ERROR', 'collision', 'already names main')], True)):
            self.assertIn('naming_lint ERROR', self.reasons()[1])


class RevertTests(Fixture):
    def test_revert_restores_exact_tokens(self):
        self.run_quiet(A.landed_main, self.args())
        git(self.root, 'add', '-A')
        git(self.root, 'commit', '-q', '-m', 'landed')
        self.run_quiet(A.revert_main, self.args())
        foo = self.text('src/main/matched/foo.c')
        self.assertIn('extern s16 D_800EF123;', foo)
        self.assertIn('/* the example object', foo)
        e = self.record()
        self.assertFalse(e['landed'])
        self.assertNotIn('landed_files', e)
        self.assertEqual(json.load(open(os.path.join(self.root, 'config/main_retail_symbols.json'))), {})
        self.assertNotIn('now_example', self.text('src/overlay/ABC/func_80132000.c'))

    def test_revert_after_a_file_moved_needs_search(self):
        self.run_quiet(A.landed_main, self.args())
        git(self.root, 'add', '-A')
        git(self.root, 'commit', '-q', '-m', 'landed')
        git(self.root, 'mv', 'src/main/matched/bar.c', 'src/main/matched/bar_renamed.c')
        git(self.root, 'commit', '-q', '-m', 'a function rename moved the file')
        out = self.run_quiet(A.revert_main, self.args())
        self.assertIn('--search', out)
        self.assertTrue(self.record()['landed'])
        self.run_quiet(A.revert_main, self.args(search=True))
        self.assertFalse(self.record()['landed'])
        self.assertIn('D_800EF123', self.text('src/main/matched/bar_renamed.c'))
        self.assertIn('D_800EF123', self.text('pc/src/globals.c'))

    def test_revert_of_an_uncommitted_landing_needs_no_git_lock(self):
        self.run_quiet(A.landed_main, self.args())      # leaves its own files git-modified
        out = self.run_quiet(A.revert_main, self.args())
        self.assertIn('locked', out)
        self.assertTrue(self.record()['landed'])
        self.run_quiet(A.revert_main, self.args(no_git_lock=True))
        self.assertFalse(self.record()['landed'])
        self.assertIn('extern s16 D_800EF123;', self.text('src/main/matched/foo.c'))

    def test_revert_needs_only(self):
        with self.assertRaises(SystemExit):
            A.revert_main(self.args(only=[]))

    def test_revert_without_landed_files_needs_search(self):
        self.run_quiet(A.landed_main, self.args())
        git(self.root, 'add', '-A')
        git(self.root, 'commit', '-q', '-m', 'landed')
        names = json.load(open(os.path.join(self.root, 'config/symbol_names.json')))
        names['symbols'][self.addr].pop('landed_files')
        write(self.root, 'config/symbol_names.json', json.dumps(names, indent=1))
        git(self.root, 'commit', '-q', '-am', 'drop landed_files')
        out = self.run_quiet(A.revert_main, self.args())
        self.assertIn('--search', out)
        self.assertTrue(self.record()['landed'])
        self.run_quiet(A.revert_main, self.args(search=True))
        self.assertFalse(self.record()['landed'])
        self.assertIn('D_800EF123', self.text('pc/src/globals.c'))


class HelperTests(unittest.TestCase):
    def test_strip_comments(self):
        s = 'a /* x\ny */ b // c\n# d\n'
        self.assertNotIn('x', A.strip_comments(s, 'f.c'))
        self.assertIn('# d', A.strip_comments(s, 'f.c'))
        self.assertNotIn('# d', A.strip_comments(s, 'f.s'))

    def test_word_re_boundaries(self):
        rx = A.word_re('D_800EE760', True)
        self.assertTrue(rx.search('x = D_800ee760;'))
        self.assertFalse(rx.search('D_800EE760_b'))
        self.assertFalse(rx.search('xD_800EE760'))
        self.assertTrue(rx.search('&D_800EE760['))

    def test_classify(self):
        self.assertEqual(A.classify('src/main/matched/a.c'), 'rename')
        self.assertEqual(A.classify('src/overlay/GEKO/f.c'), 'rename')
        self.assertEqual(A.classify('pc/include/game/globals_data.h'), 'rename')
        self.assertEqual(A.classify('Makefile'), 'rename')
        self.assertEqual(A.classify('tools/ido/x.py'), 'rename')
        self.assertEqual(A.classify('tools/overlay/custom/relocfix_DATE_80136F98.py'), 'rename')
        self.assertEqual(A.classify('pc/3rd/psyz/x.c'), 'skip')
        self.assertEqual(A.classify('docs/a.md'), 'skip')
        self.assertEqual(A.classify('src/main/matched/README.md'), 'skip')
        self.assertEqual(A.classify('config/symbol_names.json'), 'skip')
        self.assertEqual(A.classify('config/overlay/GEKO.sym'), 'skip')
        self.assertEqual(A.classify('include/psyq/libgs.h'), 'skip')
        self.assertEqual(A.classify('tools/maspsx/tests/t.py'), 'left')
        self.assertEqual(A.classify('tools/analysis/audit_main_relocs.py'), 'left')

    def test_gate_targets(self):
        with patch.object(SN, 'load_registry', lambda: {'functions': [
                {'name': 'foo', 'source': 'src/main/matched/foo.c'}]}):
            m, o, ovl, pcp = A.gate_targets(['src/main/matched/foo.c', 'src/overlay/GEKO/func_8013.c',
                                             'src/main/pc-port/p.c', 'src/main/archive/old.c'])
        self.assertEqual((m, o, ovl), (['foo'], ['GEKO/func_8013'], ['GEKO']))
        self.assertEqual(pcp, ['src/main/pc-port/p.c'])


if __name__ == '__main__':
    unittest.main()
