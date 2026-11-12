# PSY-Q GCC 2.8.1 (Linux i386 Snapshot)

This directory contains a Linux i386 snapshot of PSY-Q GCC 2.8.1-era compiler components (statically linked 32-bit executables).

These binaries are primarily used for historical investigation and compiler fingerprinting, not for the primary IDO-based matching pipeline.

## Contents

| File | Purpose |
|------|---------|
| `cc1` | C compiler frontend |
| `cpp` | C preprocessor |
| `gcc` | Driver wrapper (invokes `cc1`, etc.) |
| `as` | Assembler (if present in this snapshot) |

## Notes On Running

- These are 32-bit Linux executables. Some container/sandbox environments block 32-bit execution.
- If you need a PSY-Q GCC for experiments, prefer `tools/psyq-gcc-260/` or `tools/psyq-gcc-272/` (2.6.0/2.7.2), which are the recommended versions for the project's archived GCC investigation work.

## Related

- `tools/analysis/compiler_fingerprint.py` references these compilers for automated testing.
- The original experiment notes are kept in the project's private archive.


## Licence

These binaries are GNU GCC (Linux builds, `mips-sony-psx` target), not Sony binaries: GPL-2.0-or-later. The licence text is in `tools/licenses/GPL-2.0.txt`, and the corresponding source is listed in `tools/licenses/README.md`.
