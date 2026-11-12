#!/usr/bin/env python3
"""Audit every global function and data name for vague names.

A name is vague when it does not tell a reader what the thing is or does.
Every C/H file under src/ and include/ is scanned in parallel (one worker
per core) for the global identifiers it defines or declares: function
definitions, `extern` declarations and top-level definitions.  Those are
merged with the landed names (config/symbol_names.json, the overlay .sym
files and config/level3_functions.json), and each name gets zero or more
findings:

  address        still an address name: func_/D_/FUN_/DAT_/sub_ + hex
  addr_fragment  carries an address piece: a trailing _[0-9a-f]{4,8} or an
                 embedded 8-hex run
  family         ends in _<n> or <n> and shares its stem with >= FAMILY
                 siblings (event_voice_play_17), unless the number is a real
                 index (girl, step, case, slot, tile, bg, cw, pal ...)
  generic        every word after the scope prefix is generic (misc, work,
                 val, var, data, tbl, buf, ptr, tmp, sub, proc, func ...)
  placeholder    unk / dummy / temp / test / todo / stub / anon / foo
  short          fewer than 4 characters (not a Sony or C name)
  capitalized    a CamelCase or Capitalized name in Konami address ranges
                 (Sony API names are exempt by prefix and by registry
                 evidence)
  lib_guess      lib-prefixed guessed name of Sony statics: libxx_<host>_
                 var|ptr|tbl|str[_hex]

The scope and region of each name come from the ledger (main / live
overlay / dead overlay EVENT, GYOZI); a name only seen in source gets
"src".  Exit status is 0; the report is the product.

usage: audit_vague_names.py [--json OUT] [--jobs N] [--only CLASS,...] [--list N]
"""
import argparse
import collections
import glob
import json
import os
import re
import sys
from multiprocessing import Pool

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
if not os.path.exists(os.path.join(ROOT, 'config/symbol_names.json')):
    ROOT = os.getcwd()
DEAD = {'EVENT', 'GYOZI'}
FAMILY = 4

ADDR = re.compile(r'^(func|D|FUN|DAT|sub|jtbl|jpt|LAB|PTR|UNK)_[0-9A-Fa-f]{6,8}(_[0-9A-Fa-f]+)?$')
FRAG = re.compile(r'(_(?=[0-9a-fA-F]*[0-9])[0-9a-fA-F]{4,8}$)|((?=[0-9A-Fa-f]*[0-9])[0-9A-Fa-f]{8})')
# (a hex piece must contain a digit: `_face`, `_fade`, `_bead` are words)
NUMTAIL = re.compile(r'^(.*?)(_?)(\d+)$')
REAL_INDEX = re.compile(r'(girl|step|case|slot|tile|bg|cw|pal|mode|stage|scene|basyo|month|day|week|'
                        r'year|day|row|col|line|page|lv|level|ev|serifu|menu|layer|ch|voice|se|bgm|'
                        r'tbl|res|buf|obj|chr|item|kind|pos|frame|anim|cel|hdr|img|tex|clut|tpage|'
                        r'sprite|node|part|seq|vab|sd|cd|ot|pad|port|reg|bank|unit|player|cpu|team|'
                        r'hit|hp|sel|cursor|icon|box|win|msg|str|num|cnt|flag|bit|idx|no|dir|x|y|z|'
                        r'w|h|r|g|b|u|v|uv|xy|rgb|matrix|coord|light|fog|cam|view|rot|scale|trans|'
                        r'vec|mat|fx|wipe|fade|dummy)$')
GENERIC = {'misc', 'work', 'wk', 'val', 'value', 'var', 'data', 'dat', 'tbl', 'table', 'buf', 'buffer',
           'ptr', 'pointer', 'tmp', 'temp', 'sub', 'proc', 'func', 'fn', 'routine', 'helper', 'thing',
           'stuff', 'info', 'obj', 'object', 'item', 'entry', 'rec', 'record', 'param', 'arg', 'args',
           'res', 'result', 'state', 'status', 'flag', 'flags', 'cnt', 'count', 'num', 'idx', 'index',
           'handler', 'callback', 'cb', 'hook', 'jump', 'jmp', 'call', 'a', 'b', 'c', 'x', 'y',
           'base', 'addr', 'adr', 'mem', 'block', 'blk', 'area', 'region', 'list', 'array', 'arr', 'p',
           'q', 'n', 'i', 'j', 'k', 'lo', 'hi', 'mid', 'old', 'new', 'cur', 'prev', 'next', 'tail'}
PLACEHOLDER = re.compile(r'(^|_)(unk|unknown|dummy|temp|tmp|test|todo|stub|anon|foo|baz|xxx|placeholder)'
                         r'\d*(_|$)')
LIBGUESS = re.compile(r'^lib(gs|gte|gpu|snd|spu|cd|press|api|etc|c|math|ds|card|pad|gun|tap|mcrd)_'
                      r'(.*_(var|ptr|tbl|str|buf|val|data)_[0-9a-f]{3,}|.*_(var|val|data))$')
# (a role name that merely ends in _ptr/_tbl/_str is not a guess: only the
# old host+kind+hex shape, or a bare _var/_val/_data role, is)
SONY_PREFIX = re.compile(r'^(Gs|Gt|Ss|Spu|Cd|Pad|Dec|St|Mem|Card|Set|Get|Draw|Put|Load|Store|Move|'
                         r'Clear|Reset|Open|Close|Read|Write|Rot|Norm|Mul|Apply|Trans|Comp|Scale|Square|'
                         r'Average|Local|Ratan|Rsin|Rcos|Csin|Ccos|Sqrt|Vector|Outer|Lzc|Init|Exit|Start|'
                         r'Stop|Enter|Flush|Change|Enable|Disable|Add|Term|Test|Is|Kanji|Krom|Font|Fnt|'
                         r'Vsync|VSync|Hook|Return|Exec|Delete|Format|Undelete|Rename|Next|First|Atan|'
                         r'Sound|Ex|Sys|MDEC|RC|DMA|Malloc|Free|Calloc|Realloc|Normal|Rot|Inv|Vec|Mat|'
                         r'Color|Intpl|Dpq|Nclip|AverageZ|OuterProduct|TransMatrix|PushMatrix|PopMatrix|'
                         r'_|SD_|SE_|CD_|PCM|XA|VAB|SEQ|SEP|BIOS)')
WORD = re.compile(r'[a-z]+|[A-Z][a-z]*|\d+')
DEF_FN = re.compile(r'^(?:static\s+)?[A-Za-z_][\w\s\*]*?\b([A-Za-z_]\w*)\s*\([^;{]*\)\s*\{', re.M)
EXTERN = re.compile(r'^\s*extern\s+[^;(]*?\b([A-Za-z_]\w*)\s*(?:\[[^\]]*\])*\s*(\(|;|=)', re.M)
TOPDEF = re.compile(r'^(?!static|extern|typedef|struct|union|enum|#)[A-Za-z_][\w\s\*]*?\b([A-Za-z_]\w*)'
                    r'\s*(?:\[[^\]]*\])*\s*(=|;)', re.M)
COMMENT = re.compile(r'/\*.*?\*/|//[^\n]*', re.S)
KEYWORDS = {'if', 'while', 'for', 'switch', 'return', 'sizeof', 'else', 'do', 'goto', 'case', 'void',
            'int', 'char', 'short', 'long', 'unsigned', 'signed', 'const', 'volatile', 'u8', 's8', 'u16',
            's16', 'u32', 's32', 'register', 'auto'}


def scan(path):
    """(path, {name: kind}) for the globals PATH defines or declares."""
    try:
        t = open(path, errors='replace').read()
    except OSError:
        return path, {}
    t = COMMENT.sub(' ', t)
    out = {}
    for m in DEF_FN.finditer(t):
        out.setdefault(m.group(1), 'func')
    for m in EXTERN.finditer(t):
        out.setdefault(m.group(1), 'func' if m.group(2) == '(' else 'data')
    return path, {k: v for k, v in out.items() if k not in KEYWORDS}


def region_of(path):
    p = path.replace('\\', '/')
    m = re.match(r'src/overlay/([^/]+)/', p)
    if m:
        return ('dead:' if m.group(1) in DEAD else 'overlay:') + m.group(1)
    if p.startswith('src/main/pc-port'):
        return 'pc-port'
    if p.startswith('src/main/matched/'):
        return 'main'
    if p.startswith('src/main/'):
        return 'other'          # extras / semi-matched / archive: not built
    if p.startswith('include/overlay'):
        return 'include/overlay'
    return 'include' if p.startswith('include') else 'other'


def words(name, scope_prefix):
    w = [x.lower() for x in WORD.findall(name)]
    if scope_prefix and w and w[0] == scope_prefix:
        w = w[1:]
    return w


def classify(name, kind, region, addr, fam_size, sony_names, obin):
    f = []
    if ADDR.match(name):
        return ['address']
    if name in sony_names or name in obin:
        return []
    if FRAG.search(name) and not re.search(r'(^|_)(0x)?[0-9a-f]{2}$', name):
        f.append('addr_fragment')
    m = NUMTAIL.match(name)
    if m and fam_size.get(m.group(1).rstrip('_'), 0) >= FAMILY:
        stem = m.group(1).rstrip('_')
        last = stem.split('_')[-1].lower() if stem else ''
        if not REAL_INDEX.search(last):
            f.append('family')
    if PLACEHOLDER.search(name.lower()):
        f.append('placeholder')
    if LIBGUESS.match(name):
        f.append('lib_guess')
    scope = region.split(':')[-1].lower() if ':' in region else ''
    pre = {'event': 'event', 'gyozi': 'gyozi'}.get(scope, scope.split('_')[0] if scope else '')
    w = [x for x in words(name, pre) if not x.isdigit()]
    if w and all(x in GENERIC for x in w):
        f.append('generic')
    if len(name) < 4:
        f.append('short')
    if (name[:1].isupper() and not SONY_PREFIX.match(name) and region in ('main', 'include')
            and (addr is None or addr < 0x80086000 or addr >= 0x800EC000)):
        f.append('capitalized')
    return f


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--json')
    ap.add_argument('--jobs', type=int, default=os.cpu_count() or 4)
    ap.add_argument('--only', default='')
    ap.add_argument('--list', type=int, default=8, help='examples per class and region')
    a = ap.parse_args()
    os.chdir(ROOT)
    files = sorted(glob.glob('src/**/*.[ch]', recursive=True) + glob.glob('include/**/*.h', recursive=True))
    with Pool(a.jobs) as pool:
        results = pool.map(scan, files, chunksize=64)
    seen = {}            # name -> {kind, regions:set, files:int}
    for path, names in results:
        reg = region_of(path)
        if reg in ('pc-port', 'other'):
            continue
        for n, k in names.items():
            e = seen.setdefault(n, {'kind': k, 'regions': set(), 'files': 0})
            e['regions'].add(reg)
            e['files'] += 1
    # the ledger: landed names with their address and scope
    led = {}
    obin = set()
    doc = json.load(open('config/symbol_names.json'))['symbols']
    for k, v in doc.items():
        if not v.get('landed') or not v.get('name'):
            continue
        sc, _, ad = k.rpartition(':')
        led[v['name']] = (sc or 'main', int(ad, 16), v.get('kind'))
        ev = str(v.get('evidence', '')) + str(v.get('tier', ''))
        if re.search(r'o\.bin|\bT1\b|\bT2\b|psyq|archive', ev) and 'drift' not in ev:
            obin.add(v['name'])
    sony = set()
    try:
        for f in json.load(open('config/psyq_lib_symbols.json')).get('symbols', []):
            sony.add(f['name'] if isinstance(f, dict) else f)
    except (OSError, ValueError, AttributeError, KeyError, TypeError):
        pass
    for f in json.load(open('config/level3_functions.json'))['functions']:
        if int(f['address'], 16) >= 0x80086000 and SONY_PREFIX.match(f['name']):
            sony.add(f['name'])
    names = {}
    for n, e in seen.items():
        sc = led.get(n)
        if sc:
            region = 'main' if sc[0] == 'main' else ('dead:' if sc[0] in DEAD else 'overlay:') + sc[0]
            names[n] = (sc[2] or e['kind'], region, sc[1])
        else:
            r = sorted(e['regions'])
            names[n] = (e['kind'], r[0], None)
    for n, sc in led.items():
        if n not in names:
            region = 'main' if sc[0] == 'main' else ('dead:' if sc[0] in DEAD else 'overlay:') + sc[0]
            names[n] = (sc[2] or 'data', region, sc[1])
    fam = collections.Counter()
    for n in names:
        m = NUMTAIL.match(n)
        if m:
            fam[m.group(1).rstrip('_')] += 1
    only = set(filter(None, a.only.split(',')))
    rows = []
    for n, (kind, region, addr) in sorted(names.items()):
        f = classify(n, kind, region, addr, fam, sony, obin)
        if only:
            f = [x for x in f if x in only]
        if f:
            rows.append({'name': n, 'kind': kind, 'region': region,
                         'addr': '0x%08X' % addr if addr is not None else None, 'classes': f})
    by = collections.defaultdict(collections.Counter)
    ex = collections.defaultdict(list)
    for r in rows:
        grp = r['region'].split(':')[0]
        for c in r['classes']:
            by[c][grp + '/' + r['kind']] += 1
            if len(ex[c]) < a.list:
                ex[c].append(r['name'])
    print('%d files scanned on %d cores; %d global names; %d flagged' % (len(files), a.jobs, len(names), len(rows)))
    for c in ('address', 'addr_fragment', 'family', 'generic', 'placeholder', 'short', 'capitalized', 'lib_guess'):
        if c in by:
            print('  %-14s %5d  %s' % (c, sum(by[c].values()),
                                        ', '.join('%s %d' % kv for kv in sorted(by[c].items()))))
            print('                 e.g. ' + ', '.join(ex[c]))
    if a.json:
        json.dump(rows, open(a.json, 'w'), indent=1)
        print('wrote', a.json)


if __name__ == '__main__':
    main()
