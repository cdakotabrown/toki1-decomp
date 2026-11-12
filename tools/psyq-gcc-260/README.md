# PSY-Q GCC 2.6.0 (Linux i386 Snapshot)

This directory contains a Linux i386 snapshot of PSY-Q GCC 2.6.0-era compiler components (statically linked 32-bit executables).

This toolchain was used during the project's archived "GCC investigation" work and is referenced by `tools/analysis/compiler_fingerprint.py`.

## Contents

| File | Purpose |
|------|---------|
| `cc1` | C compiler frontend (PSY-Q patched) |
| `cc1-psx-26` | GNU C 2.6.3 Psy-Q build (`@Compiler: gcc263`) |
| `cc1-nocj` | 2.6.0 rebuilt with `-fno-cross-jumping` (`tools/patches/patches/gcc-260-no-cross-jumping.patch`) |
| `cc1-psx-26-patched` | 2.6.3 rebuilt from source with two gated flags (`@Compiler: gcc263p`, see below) |
| `build_cc1_psx_26_patched.sh` | reproducible build of `cc1-psx-26-patched` |
| `cpp` | C preprocessor |
| `gcc` | Driver wrapper (invokes `cc1`, etc.) |

## Typical Usage (cc1 Pipeline)

The most reproducible workflow is to run `cc1` directly (mirroring the project's archived experiments):

```bash
# 1) Preprocess
mipsel-linux-gnu-cpp -I include -I include/psyq -lang-c -undef source.c > preprocessed.i

# 2) Compile (cc1 reads preprocessed C from stdin)
tools/psyq-gcc-260/cc1 -quiet -O2 -G0 -mips1 -mcpu=3000 -mgas -fgnu-linker -o output.s < preprocessed.i

# 3) Assemble
mipsel-linux-gnu-as -EL -march=r3000 -mtune=r3000 -no-pad-sections -o output.o output.s
```

Notes:

- `-G0` disables small-data/`gp` optimizations (the project standard).
- `-mgas` emits GNU as compatible assembly.
- `-fgnu-linker` influences symbol/relocation expectations in the generated assembly.

The original investigation notes are kept in the project's private archive.

## Notes On Running

These are 32-bit Linux executables. Some container/sandbox environments block 32-bit execution. If the binaries crash with `Bad system call`, run on a host environment that permits i386 execution.


## `cc1-psx-26-patched` (`@Compiler: gcc263p`)

GNU C 2.6.3 for `mips-sony-psx`. It is rebuilt from the GNU source, and with
its extra flags off its output equals `cc1-psx-26`. Three flags are added,
all default off:

- `-fstack-args-first`: in `calls.c`, `expand_call` stores the stack-passed
  arguments (`store_one_arg`) before it precomputes the register arguments.
- `-fsched2-forward`: in `sched.c`, after reload, each basic block is
  re-ordered by a forward (top-down) list scheduler. It uses the same
  dependence graph and MIPS unit model as the backward scheduler. The rule:
  among the ready insns, take the longest latency-weighted path to the end
  of the block; break a tie in favour of the insn data-dependent on the one
  issued last, then by original order. A cycle with nothing ready is a
  stall and emits nothing. Blocks with `SCHED_GROUP_P` insns or interior
  notes keep the backward order.
- `-fsched-old-blockage`: both schedulers (sched1 and sched2) use function
  unit blockage values computed with the gcc 2.5 `genattrtab` formula,
  READY-COST (C) - (READY-COST (E) - 1), instead of 2.6's
  READY-COST (E) - (READY-COST (C) - 1). On the MIPS `memory` unit that
  makes a store's blockage range [1,1] instead of [1,3], as in the gcc-2.5.7
  binary. `schedule_select`'s potential-hazard tie-break then no longer
  pulls a store ahead of an equal-priority ALU insn. `genattrtab` writes a
  second table, `function_units_old[]`, and `sched.c` selects it by the
  flag.

Measurements, the census and the evidence are in
`docs/reference/MATCHING-TRICKS.md`, section "`gcc263p`".

### Build recipe

```bash
tools/psyq-gcc-260/build_cc1_psx_26_patched.sh /tmp/gcc263p-build --install
```

The script does the following:

1. Downloads `https://ftp.gnu.org/old-gnu/gcc/gcc-2.6.3.tar.bz2` and checks
   its sha256.
2. Applies the decompals/old-gcc `gcc-2.6.3-psx` steps. That is the
   `varargs.h` to `stdarg.h` sed plus the obstack, sdbout and `psx-2.5.7`
   patches. The patches are vendored in `tools/patches/patches/old-gcc-*`.
3. Applies `gcc-263-psx-gpopt.patch`. This puts `MASK_GPOPT` into
   `TARGET_DEFAULT`, which makes the `-mgpOPT` defaults line match Psy-Q's.
4. Applies `gcc-263-psx-flags.patch`, which adds `-fstack-args-first` and
   `-fsched2-forward`, then `gcc-263-psx-old-blockage.patch`, which adds
   `-fsched-old-blockage` (`genattr.c`, `genattrtab.c`, `sched.c`,
   `flags.h`, `toplev.c`).
5. Configures `--target=mips-sony-psx --host=i386-pc-linux`.
6. Overrides `config.h` for an LP64 host: `HOST_BITS_PER_LONG` 64, with
   `HOST_WIDE_INT` kept at 32-bit `int` as on the i386 original.
7. Runs `make cc1` with host gcc, statically linked, using `-std=gnu89 -O1
   -fno-strict-aliasing -fwrapv -fcommon`.

A clean run of the script reproduces the installed binary byte for byte.

To validate after a rebuild, check that every `@Compiler: gcc*` source
compiles to the same `.s` under `cc1-psx-26` and `cc1-psx-26-patched`, with
the flags off.

## Licence

These binaries are GNU GCC (Linux builds, `mips-sony-psx` target), not Sony binaries: GPL-2.0-or-later. The licence text is in `tools/licenses/GPL-2.0.txt`, and the corresponding source is listed in `tools/licenses/README.md`.
