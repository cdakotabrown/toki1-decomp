/* @SymbolSize: 384 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=-1024 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=24:s5,9:s7 */
/* @CompileEnv: IDO52_UGEN_TREG_ORDER=t7,t8,t9,t0,t1,t2,t3,t4,t5,t6 */
/* @CompileEnv: IDO52_UGEN_NO_TARGET=4 */
/* Address: 0x801356C8, Size: 384 bytes (96 insns) */
/* - `@SymbolSize: 384`.  TT.sym's `0x801356C8:424` is a MERGED BLOB; the */
/* @PostProc: scheduler_normalizer --block-aware --fence 20-95 */
/* @PostProc: swap_instructions --offset1 0x138 --offset2 0x144 */
/* @PostProc: patch_insn_reg_field 0x2c:rd:t9:t6 0x48:rs:t9:t6 0x130:rt:t0:t9 0x134:rt:t9:t2 0x138:rt:t9:t2 0x13c:rd:t1:t0 0x140:rd:t2:t1 0x140:rs:t0:t9 0x140:rt:t1:t0 0x144:rt:t2:t1 */
#include "types.h"

extern u8 *tt_boss_wk;

extern s32 rsin(s32 arg);
extern s32 rcos(s32 arg);

void tt_court_layout(s32 a, s32 b, s32 c)
{
    u8 *base;
    u8 *p;
    u8 *u;
    s32 xoff;
    s32 yoff;
    s32 x;
    s32 i;
    s32 d;
    s32 idx;
    s32 arg;
    s32 v;

    base = tt_boss_wk;
    yoff = (s16)b - 24;
    xoff = (s16)a;
    x = c & 0xFFF;
    i = 16;
    d = 85;
    p = base + 16;
    u = base + 160;

    do {
        idx = (x + 64) & 0xFFF;
        idx = idx / 128;
        arg = -1024 - x;
        u[62] = (u8)idx;
        v = rcos(arg);
        *(s16 *)(p + 632) = (s16)yoff;
        *(s16 *)(p + 584) = (s16)(v / d + xoff);
        if (idx >= 8 && idx < 25) {
            v = rsin(arg);
            *(s16 *)(p + 632) = (s16)(*(s16 *)(p + 632) + v / 128);
            *(s16 *)(p + 680) = 7;
        } else {
            v = rsin(arg);
            *(s16 *)(p + 680) = 9;
            *(s16 *)(p + 632) = (s16)(*(s16 *)(p + 632) + v / 171);
        }
        i += 2;
        u += 20;
        p += 2;
        x += 512;
    } while (i != 32);
}
