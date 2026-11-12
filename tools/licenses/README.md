# Licences of the vendored GPL tool binaries

The GPL binaries listed below are vendored so that a checkout builds offline.
The GNU General Public License, version 2, is in `GPL-2.0.txt` in this
directory. GCC 2.x is "version 2 or (at your option) any later version".
mkpsxiso and dumpsxiso are GPL-2.0.

None of these programs is linked into anything the project ships. They are
build tools. The project's own code stays MIT (`LICENSE`).

## GNU GCC 2.x with the PlayStation target patches

The binaries are:

- `tools/psyq-gcc-260/{cc1,cc1-psx-26,cc1-nocj,cc1-psx-26-patched,cpp,gcc}`
- `tools/psyq-gcc-272/{cc1,cc1-nocj,cpp,gcc}`
- `tools/psyq-gcc/{cc1,cpp,gcc}`

These are GNU GCC 2.6.0, 2.6.3, 2.7.2 and 2.8.1, built for Linux and
targeting `mips-sony-psx`. They are not Sony or SN Systems binaries. Their
version banners read `GNU C version 2.x ... compiled by GNU C version 9.4.0`
(13.3.0 for the two `cc1-nocj`, 15.2.0 for `cc1-psx-26-patched`),
so they were built from source on a modern Linux host.
`tools/psyq-gcc/as` is a project-written shell wrapper around maspsx.

Corresponding source:

- GNU GCC release tarballs: <https://ftp.gnu.org/old-gnu/gcc/>
  (`gcc-2.6.0`, `gcc-2.6.3`, `gcc-2.7.2`, `gcc-2.8.1`).
- The PlayStation target patches and build recipes:
  [decompals/old-gcc](https://github.com/decompals/old-gcc)
  (`gcc-2.6.0-psx`, `gcc-2.6.3-psx`, `gcc-2.7.2-psx`, `gcc-2.8.1-psx`).
- This project's own patches: `tools/patches/patches/*.patch`.
  - `gcc-260-no-cross-jumping.patch` adds `-fno-cross-jumping`. The
    `cc1-nocj` builds of 2.6.0 and 2.7.2 are the same change applied to each
    release.
  - The `old-gcc-*` and `gcc-263-*` patches build `cc1-psx-26-patched`.
  - The reproducible build is
    `tools/psyq-gcc-260/build_cc1_psx_26_patched.sh`. It downloads the GNU
    tarball, checks its sha256 and applies the patches.

If you received one of these binaries from this repository and cannot get the
source from the links above, open an issue on the repository and the source
will be provided.

## mkpsxiso / dumpsxiso 2.10

The binaries are `tools/psxiso/{mkpsxiso,dumpsxiso}`.

- © 2017-2022 John "Lameguy" Wilbert Villamor (Lameguy64), with Silent,
  Chromaryu, G4Vi, spicyjpeg and Phoenix.
- Licensed under GPL-2.0 (`GPL-2.0.txt`).
- Source: <https://github.com/Lameguy64/mkpsxiso>, release 2.10. These are
  unmodified Linux builds.
