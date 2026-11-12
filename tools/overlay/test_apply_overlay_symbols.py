#!/usr/bin/env python3
"""Tests for the overlay name lander (apply_overlay_symbols.py), the overlay
name table (overlay_names.py), the resolver hook-up and symbol_names.py check's
overlay invariants.  Everything runs against a throw-away git repository, so
no tracked file is touched:

    python3 tools/overlay/test_apply_overlay_symbols.py
"""
import contextlib
import io
import json
import os
import shutil
import subprocess
import sys
import tempfile
import unittest
from unittest import mock

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(HERE)), 'tools', 'analysis'))
import apply_overlay_symbols as AOS    # noqa: E402
import overlay_names as ON              # noqa: E402
import symbol_names as SN               # noqa: E402

AAA_SYM = """# AAA.EXN symbol map
# Format: address:size:name

# === Functions ===
0x80132000:52:func_80132000
0x80132040:160:func_80132040
0x801320E0:64:func_801320E8
0x80132200:32:func_80132200
"""
BBB_SYM = """# BBB.EXN symbol map
0x80132040:80:func_80132040
0x80132100:32:func_80132100
"""


def write(root, path, text):
    full = os.path.join(root, path)
    os.makedirs(os.path.dirname(full), exist_ok=True)
    with open(full, 'w', newline='') as fh:
        fh.write(text)


def dump(obj, indent=2, nl=''):
    return json.dumps(obj, indent=indent) + nl


def make_repo(root):
    """A miniature tree: two overlays that both own a func_80132040 (different
    functions), a main function, and every registry file the lander rewrites."""
    write(root, 'config/overlay/AAA.sym', AAA_SYM)
    write(root, 'config/overlay/BBB.sym', BBB_SYM)
    write(root, 'src/overlay/AAA/func_80132040.c',
          '/* func_80132040 - AAA entry */\nextern void func_80132200(void);\n'
          'void func_80132040(void)\n{\n    func_80132200();\n    func_80132200();\n}\n')
    write(root, 'src/overlay/AAA/func_80132200.c',
          '/* AAA helper; called from func_80132040 and func_80132040 (twice) */\n'
          'void func_80132200(void)\n{\n}\n')
    write(root, 'src/overlay/AAA/func_80132000.c', 'void func_80132000(void)\n{\n}\n')
    write(root, 'src/overlay/AAA/func_801320E8.c',
          'extern void func_80132040(void);\n/* func_80132040x is not it; D_func_80132040 neither */\n'
          'void func_801320E8(void)\n{\n    func_80132040();\n}\n')
    write(root, 'src/overlay/BBB/func_80132040.c',
          '/* BBB func_80132040: a DIFFERENT function with the same address name */\n'
          'void func_80132040(void)\n{\n}\n')
    write(root, 'src/overlay/BBB/func_80132100.c',
          'extern void func_80132040(void);\nvoid func_80132100(void)\n{\n    func_80132040();\n}\n')
    write(root, 'asm/overlay/AAA/func_80132040.s',
          '.globl func_80132040\n/* Function: func_80132040\n * Address: 0x80132040\n * Size: 160 bytes\n */\n'
          'func_80132040:\n    jr ra\n    nop\n')
    write(root, 'asm/overlay/BBB/func_80132040.s', '.globl func_80132040\nfunc_80132040:\n    jr ra\n    nop\n')
    status = {'AAA': {'func_80132000': {'resolved': True, 'status': 'byte_perfect'},
                      'func_80132040': {'resolved': True, 'status': 'byte_perfect'},
                      'func_801320E8': {'status': 'reloc_only', 'exact_pct': 99.0},
                      'func_80132200': {'resolved': True, 'status': 'byte_perfect'}},
              'BBB': {'func_80132040': {'resolved': True, 'status': 'byte_perfect'},
                      'func_80132100': {'resolved': True, 'status': 'byte_perfect'}}}
    write(root, 'config/overlay_match_status.json', dump(status))
    debt = {'AAA': {'from_c': ['func_80132040', 'func_80132200'], 'n': 2},
            'BBB': {'from_c': ['func_80132040', 'func_80132100'], 'n': 2}}
    write(root, 'config/clean_build_debt.json', dump(debt, nl='\n'))
    conv = {'summary': {'n': 2},
            'functions': {'AAA::func_80132040': {'overlay': 'AAA', 'name': 'func_80132040',
                                                 'c_file': 'src/overlay/AAA/func_80132040.c'},
                          'BBB::func_80132040': {'overlay': 'BBB', 'name': 'func_80132040',
                                                 'c_file': 'src/overlay/BBB/func_80132040.c'}}}
    write(root, 'config/overlay_conversion_status.json', dump(conv))
    cats = {'rows': [{'overlay': 'AAA', 'func': 'func_80132040', 'exact_pct': 1.0},
                     {'overlay': 'BBB', 'func': 'func_80132040', 'exact_pct': 2.0}]}
    write(root, 'config/overlay_decomp_categories.json', dump(cats))
    write(root, 'config/c_unmatchable.txt',
          '# header\nAAA/func_80132200   # BLOCKER_X\nBBB/func_80132040   # other\n')
    write(root, 'config/level3_functions.json', json.dumps({'functions': [
        {'name': 'main_thing', 'address': '0x80050000', 'size': 16, 'source': 'src/main/matched/main_thing.c'}]}))
    write(root, 'config/main_symbol_boundaries.json', json.dumps({'boundaries': {}}))
    write(root, 'src/main/matched/main_thing.c', 'void main_thing(void)\n{\n}\n')
    write(root, 'include/common.h', '/* nothing */\n')
    write(root, 'pc/src/overlay_AAA.c',
          '/* --- func_80132040 --- */\ns32 AAA_func_80132040()\n{\n    AAA_func_80132200();\n}\n')
    write(root, 'pc/src/overlay_BBB.c', 's32 BBB_func_80132040()\n{\n}\n')
    names = {'version': 1, 'symbols': {
        '0x80050000': {'name': 'main_thing', 'kind': 'func', 'confidence': 'LANDED', 'landed': True},
        'AAA:0x80132040': {'name': 'aaa_main', 'kind': 'func', 'scope': 'AAA', 'tier': 'T4', 'style': 'derived',
                           'confidence': 'DERIVED', 'landed': False,
                           'evidence': 'behaviour: x; reviewed:consistent (t, r): ok'},
        'AAA:0x80132200': {'name': 'aaa_helper', 'kind': 'func', 'scope': 'AAA', 'tier': 'T4', 'style': 'derived',
                           'confidence': 'DERIVED', 'landed': False,
                           'evidence': 'behaviour: y; reviewed:consistent (t, r): ok'},
        'BBB:0x80132040': {'name': 'bbb_main', 'kind': 'func', 'scope': 'BBB', 'tier': 'T4', 'style': 'derived',
                           'confidence': 'DERIVED', 'landed': False,
                           'evidence': 'behaviour: z; reviewed:consistent (t, r): ok'}},
        'hints': {}}
    write(root, 'config/symbol_names.json', json.dumps(names, indent=1) + '\n')
    for cmd in (['init', '-q'], ['add', '-A'],
                ['-c', 'user.name=t', '-c', 'user.email=t@t', 'commit', '-q', '-m', 'fixture']):
        subprocess.run(['git'] + cmd, cwd=root, check=True, capture_output=True)


def snapshot(root):
    out = {}
    for d, dirs, files in os.walk(root):
        dirs[:] = [x for x in dirs if x != '.git']
        for f in files:
            p = os.path.join(d, f)
            out[os.path.relpath(p, root)] = open(p, 'rb').read()
    return out


class Fixture(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.mkdtemp(prefix='aos_test_')
        make_repo(self.tmp)
        r = self.tmp
        self.patches = [
            mock.patch.object(AOS, 'ROOT', r),
            mock.patch.object(ON, 'ROOT', __import__('pathlib').Path(r)),
            mock.patch.object(ON, 'NAMES_FILE', __import__('pathlib').Path(r) / 'config/symbol_names.json'),
            mock.patch.object(ON, 'SYM_DIR', __import__('pathlib').Path(r) / 'config/overlay'),
            mock.patch.object(SN, 'ROOT', r),
            mock.patch.object(SN, 'NAMES', os.path.join(r, 'config/symbol_names.json')),
            mock.patch.object(SN, 'REGISTRY', os.path.join(r, 'config/level3_functions.json')),
            mock.patch.object(SN, 'BOUNDARIES', os.path.join(r, 'config/main_symbol_boundaries.json')),
        ]
        for p in self.patches:
            p.start()
        ON.load_tables(force=True)

    def tearDown(self):
        for p in reversed(self.patches):
            p.stop()
        ON.load_tables(force=True)
        shutil.rmtree(self.tmp, ignore_errors=True)

    def run_tool(self, *argv):
        out = io.StringIO()
        with mock.patch.object(sys, 'argv', ['apply_overlay_symbols.py', '--no-lint'] + list(argv)), \
                contextlib.redirect_stdout(out):
            try:
                rc = AOS.main()
            except SystemExit as e:
                rc = e.code
        return rc, out.getvalue()

    def read(self, p):
        return open(os.path.join(self.tmp, p), newline='').read()

    def exists(self, p):
        return os.path.exists(os.path.join(self.tmp, p))

    def names(self):
        return json.load(open(os.path.join(self.tmp, 'config/symbol_names.json')))['symbols']

    def check(self):
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            rc = SN.cmd_check(None)
        return rc, out.getvalue()


class TextRenameTests(unittest.TestCase):
    def test_whole_word_and_hex_case(self):
        text = 'a func_80132040 b FUNC_80132040 c func_80132040x D_func_80132040 func_8013204 func_80132040;'
        out, n = AOS.sub_words(text, {'func_80132040': 'aaa_main'})
        self.assertEqual(out, 'a aaa_main b aaa_main c func_80132040x D_func_80132040 func_8013204 aaa_main;')
        self.assertEqual(n, 3)

    def test_real_names_are_case_sensitive(self):
        out, n = AOS.sub_words('aaa_main AAA_MAIN xaaa_main', {'aaa_main': 'func_80132040'})
        self.assertEqual(out, 'func_80132040 AAA_MAIN xaaa_main')

    def test_pc_prefix_kept(self):
        out, n = AOS.sub_words('AAA_func_80132040 func_80132040 BBB_func_80132040', {'func_80132040': 'aaa_main'}, 'AAA_')
        self.assertEqual(out, 'AAA_aaa_main aaa_main BBB_func_80132040')

    def test_prefixed_only(self):
        out = AOS.prefixed_only('AAA_func_80132040 func_80132040', {'func_80132040': 'aaa_main'}, 'AAA_')
        self.assertEqual(out, 'AAA_aaa_main func_80132040')


class WalkerTests(unittest.TestCase):
    def test_context_scoping(self):
        w = AOS.Walker({'AAA': {'func_80132040': 'aaa_main'}})
        obj = {'AAA': {'func_80132040': {'status': 'x'}, 'list': ['func_80132040', 'other']},
               'BBB': {'func_80132040': {'status': 'y'}, 'list': ['func_80132040']},
               'recs': [{'overlay': 'AAA', 'func': 'func_80132040'}, {'overlay': 'BBB', 'func': 'func_80132040'}],
               'k': {'AAA::func_80132040': 1, 'BBB::func_80132040': 2, 'AAA/func_80132040x': 3},
               'path': 'src/overlay/AAA/func_80132040.c', 'path2': 'src/overlay/BBB/func_80132040.c',
               'bare': 'func_80132040'}
        new = w.walk(obj, None)
        self.assertEqual(list(new['AAA']), ['aaa_main', 'list'])      # key renamed IN PLACE
        self.assertEqual(new['AAA']['list'], ['aaa_main', 'other'])
        self.assertEqual(new['BBB']['list'], ['func_80132040'])
        self.assertEqual(new['recs'][0]['func'], 'aaa_main')
        self.assertEqual(new['recs'][1]['func'], 'func_80132040')
        self.assertEqual(sorted(new['k']), ['AAA/func_80132040x', 'AAA::aaa_main', 'BBB::func_80132040'])
        self.assertEqual(new['path'], 'src/overlay/AAA/aaa_main.c')
        self.assertEqual(new['path2'], 'src/overlay/BBB/func_80132040.c')
        self.assertEqual(new['bare'], 'func_80132040')                 # no overlay context: untouched

    def test_two_overlays_one_pass(self):
        w = AOS.Walker({'AAA': {'func_80132040': 'aaa_main'}, 'BBB': {'func_80132040': 'bbb_main'}})
        new = w.walk({'AAA': ['func_80132040'], 'BBB': ['func_80132040']}, None)
        self.assertEqual(new, {'AAA': ['aaa_main'], 'BBB': ['bbb_main']})


class LandingTests(Fixture):
    def land(self, *only, **kw):
        args = ['--apply', '--min-tier', 'DERIVED', '--no-git-lock'] + sum([['--only', o] for o in only], [])
        return self.run_tool(*args)

    def test_dry_run_changes_nothing(self):
        before = snapshot(self.tmp)
        rc, out = self.run_tool('--dry-run', '--min-tier', 'DERIVED', '--ov', 'AAA')
        self.assertEqual(rc, 0)
        self.assertIn('WOULD  AAA:0x80132040 func_80132040 -> aaa_main', out)
        self.assertEqual(before, snapshot(self.tmp))

    def test_land_scopes_to_one_overlay(self):
        rc, out = self.land('AAA:0x80132040')
        self.assertEqual(rc, 0, out)
        # AAA: file, content, asm, sym, registries
        self.assertTrue(self.exists('src/overlay/AAA/aaa_main.c'))
        self.assertFalse(self.exists('src/overlay/AAA/func_80132040.c'))
        self.assertTrue(self.exists('asm/overlay/AAA/aaa_main.s'))
        self.assertIn('void aaa_main(void)', self.read('src/overlay/AAA/aaa_main.c'))
        self.assertIn('aaa_main();', self.read('src/overlay/AAA/func_801320E8.c'))
        self.assertIn('func_80132040x is not it; D_func_80132040 neither', self.read('src/overlay/AAA/func_801320E8.c'))
        self.assertIn('called from aaa_main and aaa_main (twice)', self.read('src/overlay/AAA/func_80132200.c'))
        sym = self.read('config/overlay/AAA.sym')
        self.assertIn('0x80132040:160:aaa_main\n', sym)
        self.assertEqual(sym.replace('aaa_main', 'func_80132040'), AAA_SYM)             # order/comments kept
        # BBB is untouched (it has its OWN func_80132040)
        self.assertEqual(self.read('config/overlay/BBB.sym'), BBB_SYM)
        self.assertTrue(self.exists('src/overlay/BBB/func_80132040.c'))
        self.assertIn('func_80132040', self.read('src/overlay/BBB/func_80132100.c'))
        self.assertIn('func_80132040', self.read('asm/overlay/BBB/func_80132040.s'))
        # registries
        st = json.loads(self.read('config/overlay_match_status.json'))
        self.assertIn('aaa_main', st['AAA'])
        self.assertNotIn('func_80132040', st['AAA'])
        self.assertIn('func_80132040', st['BBB'])
        self.assertIn('aaa_main', json.loads(self.read('config/clean_build_debt.json'))['AAA']['from_c'])
        self.assertEqual(json.loads(self.read('config/clean_build_debt.json'))['BBB']['from_c'][0], 'func_80132040')
        conv = json.loads(self.read('config/overlay_conversion_status.json'))['functions']
        self.assertIn('AAA::aaa_main', conv)
        self.assertEqual(conv['AAA::aaa_main']['c_file'], 'src/overlay/AAA/aaa_main.c')
        self.assertIn('BBB::func_80132040', conv)
        cats = json.loads(self.read('config/overlay_decomp_categories.json'))['rows']
        self.assertEqual([r['func'] for r in cats], ['aaa_main', 'func_80132040'])
        self.assertIn('AAA/func_80132200', self.read('config/c_unmatchable.txt'))      # a different function
        self.assertIn('BBB/func_80132040', self.read('config/c_unmatchable.txt'))
        # the PC modules (prefixed spelling)
        self.assertIn('AAA_aaa_main()', self.read('pc/src/overlay_AAA.c'))
        self.assertIn('BBB_func_80132040()', self.read('pc/src/overlay_BBB.c'))
        # the record
        self.assertTrue(self.names()['AAA:0x80132040']['landed'])
        self.assertFalse(self.names()['BBB:0x80132040']['landed'])

    def test_structured_evidence_keeps_its_shape(self):
        doc = json.load(open(os.path.join(self.tmp, 'config/symbol_names.json')))
        doc['symbols']['AAA:0x80132040']['evidence'] = [
            {'type': 'behaviour', 'detail': 'x'}, {'type': 'review', 'detail': 'reviewed:consistent (t, r)'}]
        write(self.tmp, 'config/symbol_names.json', json.dumps(doc, indent=1) + '\n')
        rc, out = self.land('AAA:0x80132040')
        self.assertEqual(rc, 0, out)
        ev = self.names()['AAA:0x80132040']['evidence']
        self.assertIsInstance(ev, list)
        self.assertEqual(ev[-1]['type'], 'lander')
        self.assertIn('landed', ev[-1]['detail'])

    def test_json_formatting_is_preserved(self):
        self.land('AAA:0x80132200')
        # byte-for-byte: only the renamed token differs
        want = dump(json.loads(self.read('config/overlay_match_status.json')))
        self.assertEqual(self.read('config/overlay_match_status.json'), want)
        self.assertTrue(self.read('config/clean_build_debt.json').endswith('}\n'))
        self.assertFalse(self.read('config/overlay_match_status.json').endswith('\n'))

    def test_resolver_follows_the_name(self):
        self.land('AAA:0x80132040', 'BBB:0x80132040')
        self.assertEqual(ON.name_to_addr('aaa_main', 'AAA'), 0x80132040)
        self.assertEqual(ON.name_to_addr('bbb_main', 'BBB'), 0x80132040)
        self.assertIsNone(ON.name_to_addr('aaa_main', 'BBB'))                  # scoped
        self.assertEqual(ON.name_to_addr('aaa_main'), 0x80132040)              # unambiguous without a scope
        self.assertEqual(ON.name_to_addr('func_80132200', 'AAA'), 0x80132200)  # suffix rule unchanged
        self.assertEqual(ON.func_addr('AAA', 'aaa_main'), 0x80132040)
        self.assertEqual(ON.addr_name('AAA', 0x80132040), 'aaa_main')
        self.assertEqual(ON.current_name('AAA', 'func_80132040'), 'aaa_main')
        self.assertEqual(ON.current_name('BBB', 'func_80132040'), 'bbb_main')
        self.assertEqual(ON.current_name('AAA', 'func_80132200'), 'func_80132200')
        rc, out = self.check()
        self.assertEqual(rc, 0, out)

    def test_same_name_in_two_overlays_is_legal(self):
        with open(os.path.join(self.tmp, 'config/symbol_names.json')) as fh:
            doc = json.load(fh)
        doc['symbols']['BBB:0x80132040']['name'] = 'aaa_main'
        write(self.tmp, 'config/symbol_names.json', json.dumps(doc, indent=1) + '\n')
        rc, out = self.land('AAA:0x80132040', 'BBB:0x80132040')
        self.assertEqual(rc, 0, out)
        self.assertTrue(self.exists('src/overlay/AAA/aaa_main.c') and self.exists('src/overlay/BBB/aaa_main.c'))
        self.assertEqual(ON.name_to_addr('aaa_main', 'AAA'), 0x80132040)
        self.assertEqual(ON.name_to_addr('aaa_main', 'BBB'), 0x80132040)
        rc, out = self.check()
        self.assertEqual(rc, 0, out)

    def test_ambiguous_name_needs_a_scope(self):
        with open(os.path.join(self.tmp, 'config/symbol_names.json')) as fh:
            doc = json.load(fh)
        doc['symbols']['BBB:0x80132100'] = {'name': 'aaa_helper', 'kind': 'func', 'scope': 'BBB',
                                            'landed': True, 'confidence': 'DERIVED'}
        write(self.tmp, 'config/symbol_names.json', json.dumps(doc, indent=1) + '\n')
        doc['symbols']['AAA:0x80132200']['landed'] = True
        write(self.tmp, 'config/symbol_names.json', json.dumps(doc, indent=1) + '\n')
        ON.load_tables(force=True)
        self.assertEqual(ON.name_to_addr('aaa_helper', 'AAA'), 0x80132200)
        self.assertEqual(ON.name_to_addr('aaa_helper', 'BBB'), 0x80132100)
        with self.assertRaises(ON.NameTableError):
            ON.name_to_addr('aaa_helper')
        ON.set_ambient('BBB')
        try:
            self.assertEqual(ON.name_to_addr('aaa_helper'), 0x80132100)
        finally:
            ON.set_ambient(None)

    def test_revert_restores_the_tree_exactly(self):
        before = snapshot(self.tmp)
        self.land('AAA:0x80132040', 'AAA:0x80132200')
        self.assertNotEqual(before, snapshot(self.tmp))
        rc, out = self.run_tool('--revert', '--apply', '--no-git-lock', '--any-verdict', '--ov', 'AAA', '--forget')
        self.assertEqual(rc, 0, out)
        after = snapshot(self.tmp)
        # symbol_names.json: the two records are gone again, the rest is identical
        names_before = json.loads(before.pop('config/symbol_names.json'))
        names_after = json.loads(after.pop('config/symbol_names.json'))
        self.assertEqual(sorted(names_after['symbols']),
                         sorted(k for k in names_before['symbols'] if k not in ('AAA:0x80132040', 'AAA:0x80132200')))
        self.assertEqual(before, after)

    def test_revert_keeps_a_reverted_record(self):
        self.land('AAA:0x80132040')
        rc, out = self.run_tool('--revert', '--apply', '--no-git-lock', '--any-verdict', '--only', 'AAA:0x80132040')
        self.assertEqual(rc, 0, out)
        e = self.names()['AAA:0x80132040']
        self.assertFalse(e['landed'])
        self.assertEqual(e['confidence'], 'REVERTED')
        self.assertTrue(self.exists('src/overlay/AAA/func_80132040.c'))
        self.assertIn('0x80132040:160:func_80132040', self.read('config/overlay/AAA.sym'))

    def test_revert_needs_a_verdict_or_the_flag(self):
        self.land('AAA:0x80132040')
        rc, out = self.run_tool('--revert', '--apply', '--no-git-lock', '--only', 'AAA:0x80132040')
        self.assertIn('nothing revertible', out)
        self.assertTrue(self.exists('src/overlay/AAA/aaa_main.c'))


class RefusalTests(Fixture):
    def edit_record(self, key, **kw):
        doc = json.load(open(os.path.join(self.tmp, 'config/symbol_names.json')))
        doc['symbols'][key].update(kw)
        write(self.tmp, 'config/symbol_names.json', json.dumps(doc, indent=1) + '\n')

    def plan_reasons(self, key, *extra):
        rc, out = self.run_tool('--dry-run', '--min-tier', 'DERIVED', '--no-git-lock', '--only', key, *extra)
        return out

    def test_unreviewed(self):
        self.edit_record('AAA:0x80132040', evidence='behaviour: x')
        self.assertIn('no `reviewed:consistent`', self.plan_reasons('AAA:0x80132040'))

    def test_contradicted(self):
        self.edit_record('AAA:0x80132040', evidence='x; reviewed:consistent; reviewed:contradicted')
        self.assertIn('CONTRADICTED', self.plan_reasons('AAA:0x80132040'))

    def test_tier_gates(self):
        self.assertIn('below --min-tier',
                      self.run_tool('--dry-run', '--only', 'AAA:0x80132040', '--no-git-lock')[1])
        self.edit_record('AAA:0x80132040', tier='T3')
        self.assertIn('requires a T4 derived-style', self.plan_reasons('AAA:0x80132040'))

    def test_name_collisions(self):
        self.edit_record('AAA:0x80132040', name='main_thing')
        out = self.plan_reasons('AAA:0x80132040')
        self.assertIn('is a MAIN name', out)
        self.edit_record('AAA:0x80132040', name='aaa_helper')           # held by another AAA record
        self.assertIn('already held by AAA:0x80132200', self.plan_reasons('AAA:0x80132040'))
        self.edit_record('AAA:0x80132040', name='func_80132999')
        self.assertIn('address-shaped', self.plan_reasons('AAA:0x80132040'))
        self.edit_record('AAA:0x80132040', name='9bad')
        self.assertIn('not a C identifier', self.plan_reasons('AAA:0x80132040'))

    def test_shadows_main_is_a_reviewed_exception(self):
        self.edit_record('AAA:0x80132040', name='main_thing')
        self.assertIn('is a MAIN name', self.plan_reasons('AAA:0x80132040'))
        self.edit_record('AAA:0x80132040', shadows_main='overlay copy; never calls the main one')
        out = self.plan_reasons('AAA:0x80132040')
        self.assertNotIn('MAIN name', out)
        self.assertNotIn('already used as an identifier', out)       # src/main/matched/main_thing.c exists
        self.assertIn('WOULD  AAA:0x80132040', out)
        rc, out = self.check()
        self.assertEqual(rc, 0, out)
        # ... but a use inside the overlay's own tree still refuses
        write(self.tmp, 'src/overlay/AAA/func_80132200.c', 'void func_80132200(void){ main_thing(); }\n')
        self.assertIn('already used as an identifier', self.plan_reasons('AAA:0x80132040'))

    def test_name_already_in_the_overlay_tree(self):
        write(self.tmp, 'src/overlay/AAA/func_80132200.c', 'void func_80132200(void){ aaa_main(); }\n')
        self.assertIn('already used as an identifier', self.plan_reasons('AAA:0x80132040'))

    def test_same_name_in_another_overlay_is_not_a_collision(self):
        write(self.tmp, 'src/overlay/BBB/func_80132100.c', 'void func_80132100(void){ aaa_main(); }\n')
        self.assertNotIn('already used as an identifier', self.plan_reasons('AAA:0x80132040'))

    def test_name_in_main_or_include(self):
        write(self.tmp, 'include/common.h', 'extern void aaa_main(void);\n')
        self.assertIn('already used as an identifier', self.plan_reasons('AAA:0x80132040'))

    def test_missing_sym_entry(self):
        write(self.tmp, 'config/overlay/AAA.sym', AAA_SYM.replace('func_80132040', 'something_else'))
        self.assertIn('has no func_80132040 entry', self.plan_reasons('AAA:0x80132040'))

    def test_locked_files(self):
        write(self.tmp, 'src/overlay/AAA/func_801320E8.c',
              self.read('src/overlay/AAA/func_801320E8.c') + '/* someone else is editing */\n')
        rc, out = self.run_tool('--dry-run', '--min-tier', 'DERIVED', '--only', 'AAA:0x80132040')
        self.assertIn('touches locked file(s)', out)
        rc, out = self.run_tool('--dry-run', '--min-tier', 'DERIVED', '--only', 'AAA:0x80132040',
                                '--allow-locked', 'src/overlay/AAA/func_801320E8.c')
        self.assertNotIn('locked', out)
        self.assertIn('WOULD  AAA:0x80132040', out)
        rc, out = self.run_tool('--dry-run', '--min-tier', 'DERIVED', '--only', 'AAA:0x80132040', '--lock',
                                'config/overlay/AAA.sym', '--no-git-lock')
        self.assertIn('touches locked file(s)', out)

    def test_unlocked_other_overlay_edit_is_fine(self):
        write(self.tmp, 'src/overlay/BBB/func_80132100.c', '/* someone else, other overlay */\n')
        rc, out = self.run_tool('--dry-run', '--min-tier', 'DERIVED', '--only', 'AAA:0x80132040')
        self.assertIn('WOULD  AAA:0x80132040', out)

    def test_target_file_exists(self):
        write(self.tmp, 'src/overlay/AAA/aaa_main.c', 'void x(void){}\n')
        self.assertIn('target file', self.plan_reasons('AAA:0x80132040', '--allow-decl', 'src/overlay/AAA/aaa_main.c'))

    def test_already_landed(self):
        self.edit_record('AAA:0x80132040', landed=True)
        out = self.plan_reasons('AAA:0x80132040')
        self.assertIn('nothing landable', out)          # a landed record is never planned again

    def test_unreproducible_json_format_refuses_the_whole_batch(self):
        write(self.tmp, 'config/overlay_match_status.json',
              '{"AAA": {"func_80132040": {"status": "x"}},\n "BBB": {}}')
        before = snapshot(self.tmp)
        rc, out = self.run_tool('--apply', '--min-tier', 'DERIVED', '--no-git-lock', '--only', 'AAA:0x80132040')
        self.assertEqual(rc, 2, out)
        self.assertIn('cannot reproduce its JSON formatting', out)
        self.assertEqual(before, snapshot(self.tmp))               # nothing landed half-way

    def test_lint_error_refuses(self):
        # the lint is real in production; here a stub that reports an ERROR
        with mock.patch.object(AOS, 'lint_reasons', return_value=['naming_lint ERROR: made up']):
            out = io.StringIO()
            with mock.patch.object(sys, 'argv', ['x', '--dry-run', '--min-tier', 'DERIVED', '--no-git-lock',
                                                 '--only', 'AAA:0x80132040']), contextlib.redirect_stdout(out):
                AOS.main()
        self.assertIn('naming_lint ERROR: made up', out.getvalue())


class CheckTests(Fixture):
    def mutate(self, fn):
        doc = json.load(open(os.path.join(self.tmp, 'config/symbol_names.json')))
        fn(doc['symbols'])
        write(self.tmp, 'config/symbol_names.json', json.dumps(doc, indent=1) + '\n')

    def test_clean_fixture_passes(self):
        rc, out = self.check()
        self.assertEqual(rc, 0, out)
        self.assertIn('3 overlay', out)

    def test_duplicate_within_an_overlay(self):
        self.mutate(lambda s: s['AAA:0x80132200'].update(name='aaa_main'))
        rc, out = self.check()
        self.assertEqual(rc, 1)
        self.assertIn('DUP overlay name AAA:aaa_main', out)

    def test_same_name_in_two_overlays_passes(self):
        self.mutate(lambda s: s['BBB:0x80132040'].update(name='aaa_main'))
        rc, out = self.check()
        self.assertEqual(rc, 0, out)

    def test_overlay_name_may_not_equal_a_main_name(self):
        self.mutate(lambda s: s['AAA:0x80132040'].update(name='main_thing'))
        rc, out = self.check()
        self.assertEqual(rc, 1)
        self.assertIn('is also a MAIN name', out)

    def test_landed_overlay_name_needs_its_sym_entry(self):
        self.mutate(lambda s: s['AAA:0x80132040'].update(landed=True))
        rc, out = self.check()
        self.assertEqual(rc, 1)
        self.assertIn('has no AAA.sym entry', out)

    def test_scope_must_match_the_key(self):
        self.mutate(lambda s: s['AAA:0x80132040'].update(scope='BBB'))
        rc, out = self.check()
        self.assertEqual(rc, 1)
        self.assertIn('does not match its key', out)

    def test_pending_record_needs_its_func_entry(self):
        self.mutate(lambda s: s.update({'AAA:0x80139999': dict(s['AAA:0x80132040'], name='aaa_ghost', scope='AAA')}))
        rc, out = self.check()
        self.assertEqual(rc, 1)
        self.assertIn('has no func_80139999', out)

    def test_main_view_hides_overlay_records_and_save_restores_them(self):
        doc = SN.load_names()
        self.assertEqual(sorted(doc['symbols']), ['0x80050000'])
        SN.save_names(doc)
        self.assertEqual(sorted(self.names()), ['0x80050000', 'AAA:0x80132040', 'AAA:0x80132200', 'BBB:0x80132040'])

    def test_sort_order_main_first_then_overlay_by_address(self):
        doc = SN.load_names(all_scopes=True)
        doc['symbols']['0x80041000'] = {'name': 'early', 'kind': 'func', 'landed': False}
        doc['symbols']['AAA:0x80132000'] = {'name': 'a0', 'kind': 'func', 'scope': 'AAA', 'landed': False}
        SN.save_names(doc)
        self.assertEqual(list(self.names()), ['0x80041000', '0x80050000', 'AAA:0x80132000', 'AAA:0x80132040',
                                              'AAA:0x80132200', 'BBB:0x80132040'])

    def test_resolver_table_rejects_inconsistent_overlay_records(self):
        self.mutate(lambda s: s['AAA:0x80132200'].update(landed=True, name='aaa_main'))
        self.mutate(lambda s: s['AAA:0x80132040'].update(landed=True))
        ON.load_tables(force=True)
        self.assertTrue(any('landed at both' in p for p in ON.check_tables()))
        with self.assertRaises(ON.NameTableError):
            ON.name_to_addr('aaa_main', 'AAA')

    def test_overlay_name_equal_to_main_name_breaks_the_table(self):
        self.mutate(lambda s: s['AAA:0x80132040'].update(landed=True, name='main_thing'))
        ON.load_tables(force=True)
        self.assertTrue(any('ambiguous' in p for p in ON.check_tables()))


class GateWiringTests(unittest.TestCase):
    """The real tree: the gates that used to decode func_<ADDR> accept a name."""

    def test_gate_command_list_covers_the_required_gates(self):
        cmds = ' '.join(c for _, c in AOS.gate_commands({'RENSYU'}))
        for needle in ('symbol_names.py check', '--check-name-table', 'verify_overlay_bytes.py RENSYU',
                       'apply_resolved_status.py RENSYU --demote', 'audit_plain_vs_shipped.py --overlay RENSYU',
                       'prove_equivalence.py --overlay RENSYU', 'build_clean_exn.py RENSYU --verify',
                       'audit_overlay_coverage.py --overlay RENSYU', 'audit_overlay_universe.py'):
            self.assertIn(needle, cmds)

    def test_resolver_exports(self):
        import resolve_overlay_relocs as R
        self.assertIs(R.NameTableError, ON.NameTableError)
        self.assertEqual(R.name_to_addr('func_80132040', 'RENSYU'), 0x80132040)
        self.assertEqual(R.name_to_addr('func_80132040'), 0x80132040)
        self.assertIsNone(R.name_to_addr('not_an_address_name'))


if __name__ == '__main__':
    unittest.main(verbosity=1)
