#!/usr/bin/env python3
"""Retire `func_<ADDR>` spellings whose address already has a landed name.

A caller that spells `func_<ADDR>` resolves by the address in the name; a
landed name resolves through config/symbol_names.json (and, for the main
binary, the config/level3_functions.json registry).  Where both give the SAME
address the spelling is cosmetic and is rewritten to the landed name.

Scopes:
  --scope main  targets below 0x80132000.  Rewritten when the landed main
                record's KEY is the address (the record was keyed at the true
                start, head included) and the registry, if it lists the
                address, carries the same name; or when the record is keyed
                at the prologue and the registry (config/level3_functions.json)
                registers the same name at the address, up to 184 bytes
                before it (the absorbed head: the registry address is the
                function's start, and the main build resolves the name
                there; overlay references resolve through the same registry
                entry, tools/overlay/overlay_names._Tables.main_entry).  Searched in live C/H:
                src/main/matched, src/overlay/<live OV>, include/.
  --scope OV    targets inside overlay OV.  Rewritten when the address is the
                start of OV's `.sym` window named N and N is a landed function
                of OV whose name resolves to that address
                (tools/overlay/overlay_names.name_to_addr).  Searched in
                src/overlay/OV.

Refused (reported, never rewritten):
  * the file already spells the landed name (a second declaration could
    disagree with the first);
  * the address name is DEFINED on the PC side outside the generated stub
    file (a PC override keyed by that address: renaming the caller would
    bypass it).

PC edit plans (pc/tools/*_plan.json, applied by pc/tools/ps1_source_edits.py)
anchor on exact source text with occurrence counts.  For every rewritten file
that a plan entry edits, the entry's old/new strings get the same rewrite and
the counts are re-taken; the file is kept only when
rename(plans_old(source_old)) == plans_new(source_new), i.e. the PC build
sees the same text modulo the rename.  Otherwise the file is left alone
(reported as a plan refusal).

usage: retire_head_refs.py --scope main|OV (--dry-run|--apply) [--list]
"""
import argparse
import glob
import json
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, 'tools', 'overlay'))
DEAD = ('EVENT', 'GYOZI')
TOK = re.compile(r'\bfunc_(80[0-9A-Fa-f]{6})\b')


def pc_defined():
    """Addresses whose func_ name the PC port DEFINES itself: a function body
    (an override or a return-0 stub), a HAL entry macro (`TMD_LIT_PASS(func_X)`),
    or a linker-script assignment.  Renaming the PS1 callers would bypass it.
    Calls from PC override copies are fine (the generated symtab keeps a
    trampoline for every address name still referenced), and so are the
    generated `jmp <name>` alias stubs."""
    out = set()
    alias = re.compile(r'func_(80[0-9A-Fa-f]{6})\(void\) \{ __asm__\("jmp [A-Za-z_][A-Za-z0-9_]*"\); \}')
    defn = re.compile(r'\bfunc_(80[0-9A-Fa-f]{6})\s*\([^;{}()]*\)\s*\{')
    macro = re.compile(r'^\s*[A-Z][A-Z0-9_]*\(\s*func_(80[0-9A-Fa-f]{6})\b', re.M)
    ldset = re.compile(r'\bfunc_(80[0-9A-Fa-f]{6})\s*=', re.M)
    gen = re.compile(r'pc/src/overlay_[A-Z0-9_]+\.c$|pc/src/overlay_symtab\.c$|pc/include/overlay_entries\.h$')
    files = []
    for pat in ('pc/src/**/*', 'pc/hal/**/*', 'pc/include/**/*', 'pc/ld/**/*', 'src/main/pc-port/*'):
        files += [f for f in glob.glob(os.path.join(ROOT, pat), recursive=True) if os.path.isfile(f)]
    for f in files:
        if gen.search(f) or re.search(r'src/main/pc-port/func_801[3-9A-Fa-f][0-9A-Fa-f]{4}\.c$', f):
            continue            # generated, or a pc-port file pc/Makefile never builds
        text = open(f, errors='replace').read()
        if 'func_' not in text:
            continue
        text = '\n'.join(l for l in text.split('\n') if not alias.search(l))
        for rx in (defn, macro) + ((ldset,) if f.endswith(('.ld', '.syms')) else ()):
            for m in rx.finditer(text):
                out.add(int(m.group(1), 16))
    return out


def main_targets():
    syms = json.load(open(os.path.join(ROOT, 'config/symbol_names.json')))['symbols']
    reg = {int(f['address'], 16): f['name']
           for f in json.load(open(os.path.join(ROOT, 'config/level3_functions.json')))['functions']}
    by_name = {n: a for a, n in reg.items()}
    tgt = {}
    for k, v in syms.items():
        if ':' in k or not v.get('landed') or v.get('kind') != 'func' or not v.get('name'):
            continue
        a = int(k, 16)
        if a in reg and reg[a] != v['name']:
            continue
        tgt[a] = v['name']
        # a record keyed at the prologue whose registry entry starts at the
        # absorbed head: the registry address is the function's true start
        # and the build resolves the name there, so a func_<HEAD> caller
        # makes the same call (overlay C too: tools/overlay/overlay_names
        # resolves a main name to the same registry entry, _Tables.main_entry)
        r = by_name.get(v['name'])
        if r is not None and r < a <= r + 184 and r not in tgt:
            tgt[r] = v['name']
    return tgt



def ov_targets(ov):
    import overlay_names as ON
    tgt = {}
    for line in open(os.path.join(ROOT, 'config/overlay/%s.sym' % ov)):
        line = line.strip()
        if not line or line.startswith('#'):
            continue
        p = line.split(':')
        if len(p) < 3 or p[2].startswith('func_'):
            continue
        try:
            a = int(p[0], 16)
            r = ON.name_to_addr(p[2], ov)
        except Exception:
            continue
        if r == a:
            tgt[a] = p[2]
    return tgt


def plan_fix(path, old_src, new_src, sub, plans):
    """New plan list for one rewritten source, or None when the PC edits no
    longer give the same text modulo the rename."""
    import ps1_source_edits as pse
    new_plans = []
    for name, plan in plans:
        plan2 = json.loads(json.dumps(plan))
        new_plans.append((name, plan2))
    parts = path.split('/')
    stem = os.path.splitext(parts[-1])[0]
    own = ('main/' + stem) if parts[1] == 'main' else (
        '%s/%s' % (parts[2], stem) if parts[1] == 'overlay' else None)
    keys = [k for name, plan in plans for k, e in plan.get('functions', {}).items()
            if e.get('source') == path or k == own]
    if not keys:
        return plans, False
    for name, plan2 in new_plans:
        for k in set(keys):
            e = plan2.get('functions', {}).get(k)
            if e is None:
                continue
            text = None
            edits = []
            for ed in e['edits']:
                edits.append([sub(ed[0]), sub(ed[1])] + list(ed[2:]))
            e['edits'] = edits
    # recount sequentially on the evolving text, plan by plan
    for k in set(keys):
        text = new_src
        for name, plan2 in new_plans:
            e = plan2.get('functions', {}).get(k)
            if e is None:
                continue
            kept = []
            for ed in e['edits']:
                n = text.count(ed[0])
                if n == 0:
                    continue
                kept.append([ed[0], ed[1], n] if (len(ed) > 2 or n != 1) else [ed[0], ed[1]])
                text = text.replace(ed[0], ed[1])
            e['edits'] = kept
        try:
            a = sub(pse.apply_edits(k, old_src, plans))
            b = pse.apply_edits(k, new_src, new_plans)
        except ValueError:
            return None, True
        if a != b:
            return None, True
    return new_plans, True


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--scope', required=True)
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument('--dry-run', action='store_true')
    g.add_argument('--apply', action='store_true')
    ap.add_argument('--list', action='store_true')
    a = ap.parse_args()
    os.chdir(ROOT)
    if a.scope == 'main':
        tgt = main_targets()
        files = glob.glob('src/main/matched/*.c') + glob.glob('include/**/*.h', recursive=True)
        for d in sorted(glob.glob('src/overlay/*/')):
            if os.path.basename(d.rstrip('/')) not in DEAD:
                files += glob.glob(d + '*.c') + glob.glob(d + '*.h')
        inscope = lambda x: x < 0x80132000
    else:
        tgt = ov_targets(a.scope)
        files = glob.glob('src/overlay/%s/*.c' % a.scope) + glob.glob('src/overlay/%s/*.h' % a.scope)
        inscope = lambda x: x >= 0x80132000
    pcdef = pc_defined()
    sys.path.insert(0, os.path.join(ROOT, 'pc', 'tools'))
    import ps1_source_edits as pse
    plans = [(p, json.load(open(p))) for p in pse.PLANS if os.path.exists(p)]
    plans = [(os.path.relpath(p, ROOT), d) for p, d in plans]
    plans_dirty = False
    nfiles = nrefs = 0
    refused = []
    for f in sorted(files):
        t = open(f, errors='surrogateescape').read()
        rep = {}
        for m in set(TOK.findall(t)):
            x = int(m, 16)
            if not inscope(x) or x not in tgt:
                continue
            n = tgt[x]
            if x in pcdef:
                refused.append((f, m, n, 'PC spells the address name'))
            elif re.search(r'\b%s\b' % re.escape(n), t):
                refused.append((f, m, n, 'name already in file'))
            else:
                rep['func_' + m] = n
        if not rep:
            continue
        pat = re.compile(r'\b(%s)\b' % '|'.join(map(re.escape, rep)))
        sub = lambda s: pat.sub(lambda m: rep[m.group(1)], s)
        t2 = sub(t)
        np_, touched = plan_fix(f, t, t2, sub, plans)
        if np_ is None:
            refused.append((f, '*', '*', 'PC edit plan would change'))
            continue
        if touched:
            plans = np_
            plans_dirty = True
        nfiles += 1
        nrefs += len(rep)
        if a.list:
            for k, v in sorted(rep.items()):
                print('%s: %s -> %s' % (f, k, v))
        if a.apply:
            open(f, 'w', errors='surrogateescape').write(t2)
    if a.apply and plans_dirty:
        for p, d in plans:
            s = json.dumps(d, indent=2, ensure_ascii=False) + '\n'
            if open(p).read() != s:
                open(p, 'w').write(s)
                print('plan updated: %s' % p)
    for r in refused:
        print('REFUSE %s: func_%s -> %s (%s)' % r)
    print('%s %s: %d spellings in %d files, %d refused'
          % ('APPLY' if a.apply else 'DRY', a.scope, nrefs, nfiles, len(refused)))


if __name__ == '__main__':
    main()
