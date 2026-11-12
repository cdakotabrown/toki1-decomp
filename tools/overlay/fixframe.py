"""Auto-fix the patch_stack_complete --old value by trying plausible frames."""
import subprocess, re, sys

ov = 'TACO'
fn = sys.argv[1]
p = f'src/overlay/{ov}/{fn}.c'
s = open(p).read()
m = re.search(r'patch_stack_complete --old (\d+) --new (\d+)', s)
if not m:
    print('no patch_stack directive')
    sys.exit(0)
new = int(m.group(2))
for old in range(8, 160, 8):
    s2 = re.sub(r'patch_stack_complete --old \d+ --new \d+',
                f'patch_stack_complete --old {old} --new {new}', s)
    open(p, 'w').write(s2)
    r = subprocess.run(['.venv/bin/python', 'tools/overlay/resolve_overlay_relocs.py',
                        ov, fn, '-v'], capture_output=True, text=True)
    r.stdout += r.stderr
    if 'No function prologue found' in r.stdout:
        continue
    mm = re.search(r'After name-derived link: (\d+)/(\d+) words exact \(len t=(\d+) c=(\d+)\)',
                   r.stdout)
    print(f"old={old}: {mm.group(0) if mm else r.stdout.strip()[-140:]}")
    sys.exit(0)
open(p, 'w').write(s)
print('no frame worked')
