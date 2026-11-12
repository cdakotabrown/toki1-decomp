# PSY-Q GCC 2.7.2 (Linux i386 Snapshot)

This directory contains a Linux i386 snapshot of PSY-Q GCC 2.7.2-era compiler components (statically linked 32-bit executables).

This is one of the recommended PSY-Q GCC versions for the project's archived compiler fingerprinting work.

## Contents

| File | Purpose |
|------|---------|
| `cc1` | C compiler frontend (PSY-Q patched) |
| `cpp` | C preprocessor |
| `gcc` | Driver wrapper (invokes `cc1`, etc.) |

## Typical Usage (cc1 Pipeline)

```bash
# 1) Preprocess
mipsel-linux-gnu-cpp -I include -I include/psyq -lang-c -undef source.c > preprocessed.i

# 2) Compile (cc1 reads preprocessed C from stdin)
tools/psyq-gcc-272/cc1 -quiet -O2 -G0 -mips1 -mcpu=3000 -mgas -fgnu-linker -o output.s < preprocessed.i

# 3) Assemble
mipsel-linux-gnu-as -EL -march=r3000 -mtune=r3000 -no-pad-sections -o output.o output.s
```

The original command and results are kept in the project's private archive.

## Notes On Running

These are 32-bit Linux executables. Some container/sandbox environments block 32-bit execution. If the binaries crash with `Bad system call`, run on a host environment that permits i386 execution.


## Licence

These binaries are GNU GCC (Linux builds, `mips-sony-psx` target), not Sony binaries: GPL-2.0-or-later. The licence text is in `tools/licenses/GPL-2.0.txt`, and the corresponding source is listed in `tools/licenses/README.md`.
