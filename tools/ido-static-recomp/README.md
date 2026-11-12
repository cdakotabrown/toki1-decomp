# Static Recompilation of IRIX Programs

Convert selected IRIX C toolchain programs into modern Linux or macOS programs.

> **Note**: This is a modified fork for the Tokimeki Memorial decompilation project.
> See [Upstream Changes](#upstream-changes) below for differences from the original repository.

## Supported Programs

This tree vendors only the **IDO 5.2** IRIX binaries (`ido/5.2/`), which is what
the Tokimeki Memorial decompilation needs. The recompiler itself can still build
5.0/5.1/5.3/7.1 from `irix/` sources if you drop that version's binaries into
`ido/<VERSION>/` and pass `VERSION=<VERSION>` (those directories left the tree in
the 2026-11 public-release cleanup).

- IDO 5.2 (vendored): cc (driver), cfe, ugen, uopt, as1

### IDO 5.2 Status

IDO 5.2 static recompilation works after extracting the correct libc symbol addresses from IRIX 5.2's `libc.so.1`. The key insight was that IRIX 5.2 uses significantly different memory addresses for global variables (`_iob`, `errno`, `_ctype`, etc.) compared to IDO 5.3.

**Key address differences (5.2 vs 5.3):**
| Symbol | IDO 5.2 | IDO 5.3 |
|--------|---------|---------|
| \_iob | 0x0fb51bfc | 0x0fb528e4 |
| errno | 0x0fb53e20 | 0x0fb52720 |
| \_ctype | 0x0fb504c0 | 0x0fb504f0 |

**Note**: IDO 5.2 is a C89 compiler. It does not support C99 features like for-loop variable declarations (`for (int i = 0; ...)`). This is correct and expected behavior for PS1-era code.

### Where the IDO 5.2 binaries come from

`ido/5.2/` is a set of IRIX binaries collected from several sources; it was
not installed from one IRIX 5.2 disc. Each compiler pass carries an IRIX 5.2
build stamp (`cc`, `ugen`, `as1`: February 1994; `uopt`: February 1994,
another build host; `cfe`: a June 1994 IRIX 5.2 patch build). The C
libraries and `rld` carry no stamp. Nothing here shows that the set equals
any one SGI release. What shows that it is the right compiler for this game
is the build: compiled with it, the decompiled C reproduces the retail bytes.

### Recompilation fidelity

Upstream's "byte-identical to real IRIX IDO" claim is established for 5.3/7.1;
the 5.0/5.1/5.2 ports are this project's own work. The 5.2 port is checked
by running the same `ido/5.2/` binaries under `qemu-irix` and byte-comparing
their objects with the recompiled compiler's over the whole repository:
7,422 of 7,422 IDO translation units the harness compiles are identical with no `@CompileEnv` in
the environment (2026-11-11). This shows the recompilation behaves like those binaries.
It found one recomp bug (dropped prologue `sw $gp` stores that ugen later
reads back; fixed by `apply_gp_save_hooks.py`, on by default).
Harness: `tools/analysis/validate_ido52.py` (needs `make setup-qemu-irix` +
`make setup-irix-rootfs`).

## Dependencies

### Linux (Debian / Ubuntu)

```bash
sudo apt-get install build-essential
```

### macOS

[Install homebrew](https://brew.sh/) and then:

```bash
brew install make
```

## Building

First build the recomp binary itself:

```bash
make setup
```

Then build the desired IDO version:

```bash
make VERSION=5.2  # Full support - cfe, ugen, as1 (the default; what we vendor)
```

The build artifacts are located in `build/5.2/out`. Add `-j{thread num}` for multithreaded building.

By default, debug builds are created with less optimizations, debug flags, and unstripped binaries.
Add `RELEASE=1` to build release builds with optimizations and stripped binaries.

### Creating Universal ARM/x86_64 macOS Builds

By default, make build script create native binaries on macOS. This was done to minimize the time to build the recompiled suite.
In order to create "fat," universal ARM and x86_64, pass `TARGET=universal` to `gmake`.

---

## Upstream Changes

This fork differs from [decompals/ido-static-recomp](https://github.com/decompals/ido-static-recomp) in the following ways:

### Added: IDO 5.2 support

IDO 5.2 static recompilation works after extracting the correct libc symbol addresses from IRIX 5.2's `libc.so.1`.

**The key fix**: IRIX 5.2 uses different memory addresses for global variables compared to 5.3/7.1. The original code assumed 5.3 addresses, causing infinite malloc loops with 5.2 binaries.

**Files modified:**

| File          | Changes                                                                             |
| ------------- | ----------------------------------------------------------------------------------- |
| `Makefile`    | Added `VERSION=5.2` target with `IDO_VERSION := IDO52`, builds cc/cfe/ugen/uopt/as1 |
| `libc_impl.c` | Added `#ifdef IDO52` block with correct IRIX 5.2 libc addresses                     |
| `ido/5.2/`    | the IDO 5.2 binaries and libraries (provenance above)                               |

**libc_impl.c key changes (lines 62-74):**

```c
#ifdef IDO52
// IDO 5.2 - Addresses extracted from IRIX 5.2 libc.so.1 (January 2026)
// These are DIFFERENT from IDO 5.3! Incorrect addresses cause infinite malloc loops.
#define IOB_ADDR 0x0fb51bfc    // _iob / __iob (was 0x0fb528e4 in 5.3)
#define ERRNO_ADDR 0x0fb53e20  // errno - in .bss section (was 0x0fb52720 in 5.3)
#define CTYPE_ADDR 0x0fb504c0  // _ctype / __ctype (was 0x0fb504f0 in 5.3)
#define LIBC_ADDR 0x0fb50000   // .data section start (same as 5.3)
#define LIBC_SIZE 0x4000       // Must cover errno at 0x0fb53e20 (was 0x3000)
#define OPTERR_ADDR 0x0fb515a0 // opterr (was 0x0fb522e0 in 5.3)
#define OPTIND_ADDR 0x0fb515a4 // optind
#define OPTOPT_ADDR 0x0fb515a8 // optopt
#define OPTARG_ADDR 0x0fb515ac // optarg
#endif
```

### IDO 5.2 Compilation Pipeline

The full compilation pipeline works:

```
source.c → cc → cfe → ugen → as1 → output.o (ELF MIPS)
```

**Usage:**

```bash
# From project root
make setup-ido52                    # Build IDO 5.2 (one-time)
make test-ido52                     # Verify it works

# Direct usage
./build/5.2/out/cc -c source.c -o output.o
```

### IDO 5.2 Files

**Built binaries** (in `build/5.2/out/`):

| Binary | Size  | Purpose                                 |
| ------ | ----- | --------------------------------------- |
| `cc`   | 1.0MB | Compiler driver (runs the pipeline)     |
| `cfe`  | 4.2MB | C frontend (parsing, type checking)     |
| `ugen` | 2.9MB | Code generator (IR → assembly)          |
| `uopt` | 338KB | Optimizer                               |
| `as1`  | 4.3MB | Assembler (assembly → ELF)              |

**Source binaries** (in `ido/5.2/usr/`):

| Binary     | Source                   |
| ---------- | ------------------------ |
| `bin/cc`   | IRIX 5.2 compiler driver |
| `lib/cfe`  | IRIX 5.2 C frontend      |
| `lib/ugen` | IRIX 5.2 code generator  |
| `lib/uopt` | IRIX 5.2 optimizer       |
| `lib/as1`  | IRIX 5.2 assembler       |

**Libraries** (in `ido/5.2/lib/`):

| Library     | Purpose                                    |
| ----------- | ------------------------------------------ |
| `libc.so.1` | IRIX 5.2 C library (for symbol extraction) |
| `libm.so`   | Math library                               |
| `libmx.so`  | Extended math library                      |
| `rld`       | Runtime linker                             |

### How the Fix Was Found

The original 5.2 support used 5.3's libc addresses, which caused `wrapper_malloc` to read garbage from wrong memory locations. The free list pointer always read as 0, triggering infinite 64KB `sbrk` allocations.

**Solution**: Extract actual symbol addresses from IRIX 5.2's `libc.so.1` using:

```python
# Parse IRIX ELF symbol table (big-endian MIPS)
# Key symbols: _iob, errno, _ctype, opterr, optind, optopt, optarg
```

### Modularity

The 5.2 additions are fully modular:

- **No changes to 5.3/7.1 code paths** - all 5.2 code is guarded by `#ifdef IDO52`
- **Separate binary directory** - `ido/5.2/` doesn't affect other versions
- **Clean removal** - delete `ido/5.2/` and remove the VERSION=5.2 section from Makefile

---

## Original Repository

Upstream: https://github.com/decompals/ido-static-recomp

This fork is used by the Tokimeki Memorial PS1 decompilation project.
