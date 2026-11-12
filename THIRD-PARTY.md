# Third-party components

The project's own code and documentation are MIT (see `LICENSE`). Everything
below is someone else's, and keeps its own terms. This list exists so the
boundary is checkable rather than remembered: a component is either here with
a licence, or it is not in the tree at all.

This repository is toki1-decomp. The English patch (toki1-english) and the
native PC port (toki1-pc, GPL-3.0-or-later) are separate repositories with
their own THIRD-PARTY.md.

## The game

**Tokimeki Memorial: Forever with You** (SLPS-00065, Konami, 1995) is © Konami;
"Tokimeki Memorial" is a Konami trademark. The project is not affiliated with
Konami or Sony. The repository holds no disc images and none of the game's data
files, Japanese script text, art, audio or video. The disc-derived inputs are generated on your
machine from your own disc (`README.md`, "Disc"), and the paths that hold
them are gitignored. Those inputs are the retail EXNs, the main binary, the asset
tree, and the overlay disassembly. What the repository does hold: the C
reimplementation, decompiled Sony SDK functions, symbol maps, some initial
values, a few functions kept as assembly, and disassembly quoted in docs.

## Build toolchain, vendored

The build is byte-exact, so it pins exact compilers. They are vendored rather than
downloaded so that a checkout builds offline; that is also why the licence
situation has to be stated plainly.

| Component | Where | Terms | Why it is here |
|---|---|---|---|
| **IDO 5.2** (MIPS RISCompiler: `cfe`, `uopt`, `ugen`, `as1`, `cc`, IRIX libs) | `tools/ido-static-recomp/ido/5.2/` (7 MB) | Proprietary SGI/MIPS code, © Silicon Graphics. It is not covered by this repository's MIT licence, and SGI has granted no redistribution licence. The project keeps it, as other IDO-based decompilations do with their compiler binaries, because the recompiled compiler is built from these binaries and `tools/analysis/validate_ido52.py` runs them under qemu-irix. They were collected from several sources, not installed from one IRIX 5.2 disc; each compiler pass carries an IRIX 5.2 build stamp (`cfe` is a June 1994 IRIX 5.2 patch build). Anyone re-hosting the repository should know these files are SGI's, not the project's. | recompiler input; reference for the recompilation check |
| **ido-static-recomp** (fork) | `tools/ido-static-recomp/` | Upstream: <https://github.com/decompals/ido-static-recomp>. **Upstream publishes no licence.** There is no LICENSE file in the upstream repository, and GitHub reports none (checked 2026-11-06). The same is true of this vendored copy. This project's additions (the 5.0/5.1/5.2 ports, the `apply_*_hooks.py` appliers) are MIT; the upstream parts have no stated licence. `tools/ido-static-recomp/tools/rabbitizer/` is MIT (its own LICENSE). | builds the IDO 5.2 recompiler the build uses |
| **GNU GCC 2.6.0 / 2.6.3 / 2.7.2 / 2.8.1, PlayStation target** (`cc1`, `cc1-nocj`, `cc1-psx-26`, `cc1-psx-26-patched`, `cpp`, `gcc`) | `tools/psyq-gcc-260/`, `tools/psyq-gcc-272/`, `tools/psyq-gcc/` | **GPL-2.0-or-later** (GNU GCC), `tools/licenses/GPL-2.0.txt`. These are Linux builds of GNU GCC 2.x with the `mips-sony-psx` patches (decompals/old-gcc style), not Sony/SN binaries. Corresponding source: the GNU tarballs, decompals/old-gcc and `tools/patches/patches/`, all listed in `tools/licenses/README.md`. | compiles the Psy-Q GCC region |
| **mkpsxiso / dumpsxiso** 2.10 | `tools/psxiso/` | **GPL-2.0**, © Lameguy64 and contributors, `tools/licenses/GPL-2.0.txt`. Unmodified Linux builds. Source: <https://github.com/Lameguy64/mkpsxiso> (`tools/licenses/README.md`). | disc image extract and rebuild |
| **maspsx** | `tools/maspsx/` | MIT, © 2023 Mark Street — `tools/maspsx/LICENSE` | assembles MASPSX-style GCC output |
| **asm-differ** | `tools/asm-differ/` | Public domain / Unlicense — `tools/asm-differ/LICENSE` | diffing compile output against the target |
| **ASPSX driver** | `tools/aspsx/` | Project-written wrapper (`aspsx.sh`). The Psy-Q ASPSX assembler binary it drives is Sony/SN's, and it is **not** in the tree; supply your own. | assembler front end |

## Everything else

- `include/gte_macros.inc` (and its copy in `tools/decomp/permuter/include/`): a
  factual table of the GTE (COP2) opcode encodings as GAS macros, per psx-spx.
  The same macro set circulates among PS1 decompilation projects; this copy's
  exact source was not recorded when it was added (2026-01-19). Not Sony code.
- `include/psyq/libgte.h`, `include/psyq/stddef.h`: project-written headers that
  stand in for the two Psy-Q headers the matched C includes
  (`include/psyq/README.md`). Sony's SDK headers themselves are not in the tree.
- `assets/`, `disks/`, `tools/assets/jpsxdec_v2.0/`: never tracked. See `.gitignore`.

If you find a third-party file that is not on this list, that is a bug: add it with
its terms or take it out.