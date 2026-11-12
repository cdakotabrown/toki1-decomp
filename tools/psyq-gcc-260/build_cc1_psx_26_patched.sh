#!/bin/bash
# Rebuild tools/psyq-gcc-260/cc1-psx-26-patched from GNU gcc-2.6.3 source.
#
# The result is GNU C 2.6.3 for mips-sony-psx (the decompals/old-gcc
# "gcc-2.6.3-psx" configuration, built as an x86_64 LP64 binary with
# HOST_WIDE_INT kept 32-bit) plus three gated, default-off decision flags:
#   -fstack-args-first  expand_call stores the stack arguments before it
#                       computes the register arguments
#   -fsched2-forward    after reload, each block is re-ordered by a forward
#                       list scheduler (critical-path priority, then the
#                       insn data-dependent on the last one issued, then
#                       original order) over the same dependence graph and
#                       MIPS unit model
#   -fsched-old-blockage  both schedulers use the function-unit blockage
#                       values of the gcc 2.5 genattrtab formula (a store
#                       blocks the MIPS memory unit for 1 cycle, not 3), so
#                       the potential-hazard tie-break no longer pulls a
#                       store ahead of an ALU insn; genattrtab emits both
#                       tables and the flag picks one
# With no flag it emits the same .s as the shipped cc1-psx-26
# (verified on every @Compiler: gcc* source in the tree).
# See tools/psyq-gcc-260/README.md.
#
# usage: build_cc1_psx_26_patched.sh WORKDIR [--install]
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
PATCHES="$ROOT/tools/patches/patches"
WORK="${1:?usage: $0 WORKDIR [--install]}"
URL=https://ftp.gnu.org/old-gnu/gcc/gcc-2.6.3.tar.bz2
SHA=71f673849f4dc7f20aba6ba1ecb6221c6704ef3152779aef15602e974ae3c140

mkdir -p "$WORK"; cd "$WORK"
[ -f gcc-2.6.3.tar.bz2 ] || curl -fsSL -o gcc-2.6.3.tar.bz2 "$URL"
echo "$SHA  gcc-2.6.3.tar.bz2" | sha256sum -c -
rm -rf gcc-2.6.3; tar xjf gcc-2.6.3.tar.bz2; chmod -R u+w gcc-2.6.3; cd gcc-2.6.3

# decompals/old-gcc gcc-2.6.3-psx.Dockerfile steps
sed -i -- 's/include <varargs.h>/include <stdarg.h>/g' *.c
patch -su -p1 obstack.h -i "$PATCHES/old-gcc-obstack-2.7.2.h.patch"
patch -su -p1 sdbout.c  -i "$PATCHES/old-gcc-sdbout-2.6.3.c.patch"
patch -su -p1 < "$PATCHES/old-gcc-psx-2.5.7.patch"
# Psy-Q's cc1-psx-26 reports "-mgas -mgpOPT -msoft-float" as its defaults
patch -su -p1 < "$PATCHES/gcc-263-psx-gpopt.patch"
# the gated decision flags
patch -su -p1 < "$PATCHES/gcc-263-psx-flags.patch"
patch -su -p1 < "$PATCHES/gcc-263-psx-old-blockage.patch"
touch insn-config.h

./configure --target=mips-sony-psx --prefix=/opt/cross --with-endian-little \
    --with-gnu-as --host=i386-pc-linux --build=i386-pc-linux > configure.log

# LP64 host: long and pointers are 64-bit, HOST_WIDE_INT stays 32-bit as on
# the original i386 build (same as old-gcc's xm-darwin.h).
cp --remove-destination "$(readlink -f config.h)" config.h
cat >> config.h <<'EOC'

#undef HOST_BITS_PER_LONG
#define HOST_BITS_PER_LONG 64
#define HOST_BITS_PER_WIDE_INT 32
#define HOST_WIDE_INT int
#define HAVE_STRERROR
EOC

make -j"$(nproc)" cc1 CC=gcc CFLAGS="-std=gnu89 -O1 -fno-strict-aliasing \
 -fwrapv -fcommon -w -Wno-implicit-function-declaration -Wno-int-conversion \
 -Wno-incompatible-pointer-types -Wno-return-mismatch -Wno-implicit-int \
 -static -Dbsd4_4 -Dmips -DHAVE_STRERROR" > build.log 2>&1 \
    || { tail -30 build.log; exit 1; }
echo "built $WORK/gcc-2.6.3/cc1"
if [ "${2:-}" = "--install" ]; then
    cp cc1 "$ROOT/tools/psyq-gcc-260/cc1-psx-26-patched"
    echo "installed tools/psyq-gcc-260/cc1-psx-26-patched"
fi
