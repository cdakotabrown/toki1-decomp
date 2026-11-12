import re
import subprocess

p = 'src/overlay/ETC/func_80144D7C.c'
base = open('wip-scratch/f4D7C.bak').read()

ENVS = [
    '',
    'IDO52_NO_CSE=op',
    'IDO52_NO_CSE=all',
    'IDO52_NO_CSE_OPCODES=and',
    'IDO52_CSE_MAX_REUSE=1',
    'IDO52_CSE_MAX_REUSE=0',
    'IDO52_UGEN_NUM_TREGS=0',
    'IDO52_UGEN_NUM_TREGS=1',
    'IDO52_UGEN_NUM_TREGS=2',
    'IDO52_UGEN_NUM_TREGS=6',
    'IDO52_MAX_SREGS=0',
    'IDO52_MAX_SREGS=1',
    'IDO52_EXCLUDE_REGS=a0',
    'IDO52_EXCLUDE_REGS=v0',
    'IDO52_EXCLUDE_REGS=v0,v1',
    'IDO52_NO_SREG_PROMOTE=1',
    'IDO52_NO_LICM=1',
    'IDO52_NO_STRENGTH_REDUCE=1',
    'IDO52_SKIP_PASSES=copypropagate',
    'IDO52_UGEN_TREG_ORDER=t0,t1,t2,t3,t4,t5,t6,t7,t8,t9',
    'IDO52_UGEN_TREG_ORDER=t9,t8,t7,t6,t5,t4,t3,t2,t1,t0',
    'IDO52_AS1_NO_LA_PULL=all',
    'IDO52_AS1_FORCE_LA_PULL=all',
    'IDO52_LDA_NOINREG_CLASSES=1,4',
    'IDO52_CONST_NOINREG_VALUES=255',
    'IDO52_CONST_INREG_VALUES=255',
]

for e in ENVS:
    s = base if not e else ('/* @CompileEnv: %s */\n' % e) + base
    open(p, 'w').write(s)
    r = subprocess.run(['.venv/bin/python3', 'tools/overlay/sbs.py',
                        'ETC', 'func_80144D7C'], capture_output=True, text=True)
    lines = r.stdout.splitlines()
    if not lines:
        print('%-45s FAILED %s' % (e or '(none)', r.stderr.strip()[:60]))
        continue
    m = re.search(r'words target=(\d+) compiled=(\d+)', lines[0])
    n = len([x for x in lines if x.startswith('*')])
    got_move = any('move $a0, $v0' in x.split('|', 1)[1]
                   for x in lines if '|' in x and len(x.split('|')) > 1)
    print('%-45s t=%s c=%s diffs=%-4d argmove=%s'
          % (e or '(none)', m.group(1), m.group(2), n, got_move))
open(p, 'w').write(base)
