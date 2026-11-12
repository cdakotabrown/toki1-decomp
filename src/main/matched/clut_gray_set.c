/* Address: 0x8004B8F0 */
/* Size: 488 bytes (122 instructions) */
/* Region: Game (IDO 5.2 -O2, -Wc,-pic1) */
/* @SymbolStart: 0x8004B8F0 */
/* @SymbolSize: 488 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=3:t5,4:t8,6:t9,7:t5,8:t8 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0x30:rd:t5:t8 0x34:rt:t5:t8 0x34:rd:t8:t9 0x38:rd:t5:t8 0x48:rs:t8:t9 0x4c:rt:t5:t8 0x4c:rd:t8:t9 0x50:rd:t9:t5 0x54:rt:t9:t5 0x54:rd:t9:t6 0x5c:rs:t8:t9 0x60:rd:t6:t7 0x64:rs:t9:t6 0x64:rt:t6:t7 0xb0:rd:t7:a3 0xb4:rs:t7:a3 0x15c:rt:t9:t7 0x164:rs:t9:t7 0x164:rt:t8:t9 0x194:rd:t5:a3 0x198:rs:t5:a3 0x198:rt:t6:t5 0x19c:rs:t6:t5 0x19c:rt:t7:t6 0x1a0:rs:t7:t6 0x1a0:rt:t8:t9 0x1a0:rd:t5:t8 0x1a4:rt:t5:t8 0x1a8:rt:t7:t6 0x1ac:rs:t6:t5 0x1b0:rt:t6:t5 0x1b0:rd:t6:t5 0x1b4:rs:t6:t5 0x1b4:rt:t9:t7 0x1b8:rs:t7:t6 0x1b8:rt:t8:t9 0x1bc:rs:t9:t7 0x1bc:rt:t8:t9 0x1bc:rd:t5:t8 0x1c0:rt:t5:t8 0x1c4:rt:t9:t7 0x1c8:rd:t6:t5 0x1cc:rs:t6:t5 0x1cc:rt:t7:t6 0x1d0:rs:t9:t7 0x1d0:rt:t8:t9 0x1d4:rs:t7:t6 0x1d4:rt:t8:t9 0x1d4:rd:t5:t8 0x1dc:rt:t5:t8 */
#include "types.h"

typedef struct {
    u32 d0 : 5;
    u32 m0 : 5;
    u32 y0 : 5;
    u32 pad0 : 1;
    u32 d1 : 5;
    u32 m1 : 5;
    u32 y1 : 5;
    u32 pad1 : 1;
} DatePair;

typedef struct {
    u16 a : 5;
    u16 b : 5;
    u16 c : 5;
    u16 pad : 1;
} Packed;

void clut_gray_set(DatePair *src, Packed *dst)
{
    DatePair *p;
    Packed *r;
    s32 i;
    s32 v;
    s32 q;

    p = src;
    r = dst;
    for (i = 0; i < 128; i++) {
        v = p->d0 * 30 + p->m0 * 59 + p->y0 * 11;
        if (v != 0 && v < 100) {
            v = 100;
        }
        q = v / 100;
        q = q & 0x1F;
        r[0].a = q;
        r[0].b = q;
        r[0].c = q;
        v = p->d1 * 30 + p->m1 * 59 + p->y1 * 11;
        if (v != 0 && v < 100) {
            v = 100;
        }
        q = v / 100;
        q = q & 0x1F;
        r[1].a = q;
        r[1].b = q;
        r[1].c = q;
        p++;
        r += 2;
    }
}
