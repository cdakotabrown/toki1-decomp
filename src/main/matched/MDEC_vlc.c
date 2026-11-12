/* Address: 0x80086FB0 */
/* Size: 592 bytes */
/* @SymbolStart: 0x80086FB0 */
/* @SymbolSize: 592 */
/* function (src/main/semi-matched/func_80087208.c: "@SymbolStart: */
/* Region: Library (0x80086000+), Psy-Q GCC (leaf: no `jal`, `j`/`jr` */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 -fno-cse-follow-jumps -fno-schedule-insns -fno-schedule-insns2 --fill-load-delay --fill-ptr-bump-down */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u16 CF_DVLC[];
extern u16 CF2_DVLC[];

s32 MDEC_vlc(u32 w, u16 *dst)
{
    u16 hdr0;
    u16 dcq;
    u32 entry;
    u16 *q;
    u16 *t1;
    s32 v1;
    s32 bp;
    u16 v0;
    u32 x;
    u32 i;
    u32 i2;
    u16 *tb;
    u32 hi;
    u32 lo;
    u32 dcv;

    v0 = *(u16 *)w;
    hdr0 = *(volatile u16 *)w;
    *dst = v0;
    w += 2;
    dst[1] = *(u16 *)w;
    w += 2;
    dst++;
    v1 = ((u16 *)w)[1];
    dcq = *(u16 *)w;
    __asm__ volatile ("" : : "r" (dst));
    dst++;
    w += 2;
    __asm__ volatile ("" : : "r" (w));
    w += 2;
    if (v1 == 1) goto hdr_ok;
    if (v1 != 2) {
        return 0;
    }
hdr_ok:
    hi = ((u16 *)w)[0];
    lo = ((u16 *)w)[1];
    q = (u16 *)w + 2;
    hi <<= 16;
    t1 = dst;
    w = hi | lo;
    v1 = w >> 22;
    dst++;
    bp = 10;
    entry = dcq << 10;
    w <<= 10;
    if (v1 >= 0) {
dcpos:
        dcv = entry | v1;
    } else {
dcneg:
        dcv = entry | (v1 + 1024);
    }
    *t1 = dcv;

ac_loop:
    {
        i = w >> 19;
        tb = CF_DVLC;
        entry = (u32)(tb + (i << 2));
        if (((u16 *)entry)[1] == 0) {
            i2 = w >> 15;
            tb = CF2_DVLC;
            entry = (u32)(tb + (i2 << 2));
            bp += 8;
            w <<= 8;
            if (bp & 0x10) {
                bp &= 0xf;
                w |= *q << bp;
                q++;
            }
        }
        w <<= ((u16 *)entry)[0];
        bp += ((u16 *)entry)[0];
        if (bp & 0x10) {
            bp &= 0xf;
            w |= *q << bp;
            q++;
        }

        v1 = (*dst = ((u16 *)entry)[1]);
        if (v1 == 0x7C1F) {
            goto esc;
        }
        if (v1 == 0xFE00) {
dc:
            v1 = w >> 22;
            dst++;
            if (v1 == 511) {
                goto pad;
            }
            bp += 10;
            w <<= 10;
            if (bp & 0x10) {
                bp &= 0xf;
                w |= *q << bp;
                q++;
            }
            t1 = dst;
            dst++;
            entry = dcq << 10;
            if (v1 >= 0) {
                goto dcpos;
            }
            goto dcneg;
        }
        v0 = ((u16 *)entry)[2];
        *++dst = v0;
        v1 = v0;
        if (v1 == 0x7C1F) {
            goto esc;
        }
        if (v1 < 0x7C20) {
            if (v1 == 0) {
                goto ac_loop;
            }
        } else if (v1 == 0xFE00) {
            goto dc;
        }
        v0 = ((u16 *)entry)[3];
        *++dst = v0;
        v1 = v0;
        if (v1 == 0x7C1F) {
            goto esc;
        }
        if (v1 < 0x7C20) {
            if (v1 == 0) {
                goto ac_loop;
            }
        } else if (v1 == 0xFE00) {
            goto dc;
        }
        dst++;
        goto ac_loop;
esc:
        *dst = w >> 16;
        x = *q;
        w <<= 16;
        q++;
        dst++;
        w |= x << bp;
        goto ac_loop;
    }
pad:
    for (v1 = 0; v1 < 64; v1++) {
        *dst++ = 0xFE00;
    }
    return hdr0 + 1;
}
