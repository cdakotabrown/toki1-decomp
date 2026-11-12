/* Address: 0x8008E55C */
/* Size: 192 bytes (48 instructions) */
/* Region: Library (0x80086000+) */
/* Compiler: IDO 5.2 */
/* store sink.  `@Compiler: ido52-gas` + copy-and-bump/fill-load-delay/ */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_COPYPROP_VAR=-4,-8,-12 IDO52_FORCE_WEB_COLOR_NOSPLIT=0,7,14,21 IDO52_FORCE_WEB_COLOR=0:t1,7:t2,14:t3,21:v1,31:t0 IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_DIV_MACRO=aspsx IDO52_AS1_FORCE_BOPT=0 IDO52_AS1_TIE_TAKE=42 IDO52_AS1_TIE_REJECT=23,24,26 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern void SpuVmKeyOn();

void SpuVmSeKeyOn(a0, a1, a2, a3, l, r)
    int a0, a1, a2, a3;
    unsigned short l, r;
{
    int x;
    int y;
    unsigned int z;
    unsigned short base;
    unsigned short lvl;

    x = (a0 << 16) >> 16;
    y = (a1 << 16) >> 16;
    z = a2 & 0xFFFF;
    if (l == r) {
        lvl = 64;
        base = l;
    } else if ((unsigned)r < (unsigned)l) {
        lvl = ((unsigned)r << 6) / (unsigned)l;
        base = l;
    } else {
        lvl = 127 - (((unsigned)l << 6) / (unsigned)r);
        base = r;
    }
    SpuVmKeyOn(32, x, y, z, base, lvl);
}
