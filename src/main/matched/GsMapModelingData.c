/* Function: GsMapModelingData */
/* Address: 0x8009D220 (the map records 0x8009D230, the `addiu sp,sp,-8` in */
/* Size: 112 bytes (28 instructions) */
/* Region: Library (0x80086000-0x800BFFFF), Sony LIBGS GS_105 (SCLIB.C) */
/* Compiler: Psy-Q GCC 2.6.0 -O2 + maspsx --konami-lib-sched */
/* @SymbolStart: 0x8009D220 */
/* @SymbolSize: 112 */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

void GsMapModelingData(long *p)
{
    long n, i;
    long *e;

    if (p[0] & 1) {
        return;
    }
    p[0] |= 1;
    p++;
    n = *p;
    e = p + 1;
    for (i = 0; i < n; i++) {
        e[i * 7 + 0] += (long)e;
        e[i * 7 + 2] += (long)e;
        e[i * 7 + 4] += (long)e;
    }
}
