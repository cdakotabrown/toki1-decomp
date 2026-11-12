#!/bin/bash
# dis.sh <hexaddr> <size> -- disassemble a window of the retail main binary
A=$1; S=$2
python3 - "$A" "$S" <<'PY'
import sys
a=int(sys.argv[1],16); s=int(sys.argv[2])
d=open('./disks/SLPS_000.65','rb').read()
o=(a-0x80041000)+0x800
open('/tmp/dis.bin','wb').write(d[o:o+s])
PY
mips-linux-gnu-objdump -D -b binary -m mips:3000 -EL --adjust-vma=$A /tmp/dis.bin | tail -n +7
