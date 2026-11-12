import re
import subprocess

p = 'src/overlay/ETC/func_80144D7C.c'
base = open('wip-scratch/f4D7C.bak').read()
REGS = ['v0', 'v1', 'a0', 'a1', 'a2', 'a3',
        't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7', 't8', 't9']
WEBS = [0, 2, 4, 6, 12, 27, 29]

for reg in REGS:
    for w in WEBS:
        e = 'IDO52_PRECOLOR_WEB=%s:%d' % (reg, w)
        open(p, 'w').write('/* @CompileEnv: %s */\n' % e + base)
        r = subprocess.run(['.venv/bin/python3', 'tools/overlay/sbs.py',
                            'ETC', 'func_80144D7C'],
                           capture_output=True, text=True)
        lines = r.stdout.splitlines()
        if not lines:
            continue
        m = re.search(r'words target=(\d+) compiled=(\d+)', lines[0])
        if not m:
            continue
        n = len([x for x in lines if x.startswith('*')])
        mv = any('move $a0, $v0' in x.split('|', 1)[1]
                 for x in lines if '|' in x)
        if int(m.group(2)) == int(m.group(1)) or mv or n < 74:
            print('%-8s web=%-3d c=%s diffs=%-4d argmove=%s'
                  % (reg, w, m.group(2), n, mv))
open(p, 'w').write(base)
print('done')
