/* @SymbolStart: 0x80142100 */
/* @SymbolSize: 572 */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=79:a2 */
/* Address: 0x80142100 (cross-prologue head absorbed), Size: 572 bytes */
/* - `@CompileFlags: -Wc,-pic0` -- HONEST, not a frame fudge.  The target */
/* @PostProc: remap_stack_slots --from 48 --to 52 */
/* @PostProc: patch_insn_reg_field 0x90:rt:t2:t1 0x94:rt:t1:t3 0x98:rd:t3:t2 0x98:rs:t2:t1 0x9c:rs:t1:t2 0xb0:rt:a2:t6 0xb8:rs:a2:t6 0xb8:rt:t6:t7 0xbc:rt:t6:t7 0xc0:rt:t7:t8 0xc8:rs:t7:t8 0xc8:rt:t8:t9 0xd0:rs:t8:t9 0xd8:rt:t9:t0 0xdc:rs:t6:t7 0xdc:rt:t0:t1 0xe0:rs:t9:t0 0xe0:rt:t0:t1 0xe4:rt:v0:a2 0xec:rs:v0:a2 0xec:rt:t1:t4 0xf0:rd:t3:t6 0xf0:rt:t1:t4 0xf4:rs:v0:a2 0xf4:rt:t4:t7 0xf8:rd:t5:t8 0xf8:rs:t3:t6 0xf8:rt:t4:t7 0xfc:rt:t5:t8 0x100:rt:t6:t9 0x108:rs:t6:t9 0x108:rt:t8:t1 0x10c:rt:t8:t1 0x110:rs:t8:t1 0x110:rt:t9:t2 0x118:rt:t9:t2 0x11c:rt:v0:t3 0x124:rs:v0:t3 0x124:rt:t0:t7 0x128:rt:t0:t7 0x12c:rt:a2:v0 0x130:rt:t3:t8 0x134:rd:t2:t5 0x134:rt:a2:v0 0x138:rs:t2:t5 0x138:rt:t1:t6 0x13c:rs:t3:t8 0x13c:rt:t4:t9 0x140:rd:t5:t0 0x140:rs:t4:t9 0x140:rt:t1:t6 0x144:rs:t5:t0 0x144:rt:t6:t1 0x148:rd:v0:t2 0x148:rs:t6:t7 0x148:rt:t0:t1 0x14c:rt:v0:t2 0x150:rt:t7:t4 0x154:rs:v0:t2 0x154:rt:t2:t3 0x158:rs:t7:t4 0x158:rt:t8:t5 0x15c:rd:t9:t6 0x15c:rs:t8:t5 0x15c:rt:t1:t6 0x160:rs:t9:t6 0x160:rt:t0:t8 0x164:rd:v0:t9 0x164:rs:t0:t3 0x164:rt:t2:t8 0x168:rt:v0:t9 0x16c:rt:t1:t0 0x170:rs:v0:t9 0x170:rt:t3:t1 0x174:rs:t1:t0 0x17c:rs:t3:t1 0x180:rt:t7:t6 0x194:rt:t6:t5 0x19c:rt:t4:t2 0x1a4:rs:t4:t2 0x1a4:rt:t5:t4 0x1ac:rt:t5:t4 0x1b4:rt:t6:t5 0x1b8:rt:t7:t6 0x1bc:rt:t8:t3 0x1c0:rt:t3:t1 0x1c4:rs:t8:t3 0x1c4:rt:t9:t8 0x1c8:rs:t9:t8 0x1e0:rt:t1:t7 0x1e8:rt:t0:t9 0x1f0:rs:t0:t9 0x1f0:rt:t2:t0 0x1f8:rt:t2:t0 0x200:rt:t1:t7 0x204:rt:t3:t1 0x208:rt:t5:t2 0x20c:rt:t4:t5 0x210:rd:t6:t4 0x210:rs:t5:t2 0x214:rd:t7:t6 0x214:rt:t6:t5 0x218:rt:t7:t6 0x21c:rt:v0:t9 0x224:rt:v0:t9 0x228:rt:v1:t7 0x22c:rd:t8:t0 0x22c:rs:v0:t9 0x230:rd:t9:t1 0x230:rs:t8:t0 0x230:rt:v1:t7 0x234:rt:t9:t1 0x238:rt:v1:t7 */
#include "types.h"

extern u8 *tt_game_wk;
extern u8 *tt_step_wk;

extern void pad_main(void);
extern u32 pad_get(void);

void tt_pad_read(void)
{
    u8 *h;
    u8 *g;
    u8 *pa;
    u8 *pb;
    u8 *pp;
    u32 pad;
    s32 m;
    u16 sh;
    s32 w;
    s32 t;
    u32 v;

    g = tt_game_wk;
    h = tt_step_wk;
    pa = g + 16;
    pb = g + 32;
    pad_main();
    pad = pad_get();
    pp = (u8 *)&pad;

    *(s16 *)(pa + 2) = (s16)*(u16 *)(pa + 0);
    if (g[84] != 0) {
        *(s16 *)(pa + 0) = (s16)*(u16 *)(pp + 2);
    } else {
        *(s16 *)(pa + 0) = (s16)*(u16 *)(pp + 0);
    }
    *(s16 *)(pa + 4) = (s16)(~*(u16 *)(pa + 2) & *(u16 *)(pa + 0));
    *(s16 *)(pb + 2) = (s16)*(u16 *)(pb + 0);

    m = *(u16 *)(g + 80) & 0xFFF;
    *(s16 *)(g + 80) = (s16)m;

    if (h[34] != 0) {
        v = (*(u8 **)(g + 76))[(u16)m];
        *(s16 *)(pb + 0) = (s16)(((v & 0xF0) << 8) | (v & 3));
        *(s16 *)(g + 80) = (s16)(*(u16 *)(g + 80) + 1);
        *(s16 *)(g + 80) = (s16)(*(u16 *)(g + 80) & 0xFFF);
    } else {
        w = *(u16 *)(pa + 0) & 0xFF00;
        *(s16 *)(pb + 0) = (s16)w;
        sh = (u16)(*(u16 *)(pa + 0) << 8);
        w = w | (((u32)sh >> (g[68] + 8)) & 1);
        *(s16 *)(pb + 0) = (s16)w;
        w = (u16)w | (((u32)sh >> (g[69] + 7)) & 2);
        *(s16 *)(pb + 0) = (s16)w;
        if (g[67] != 0) {
            if ((w & 1) != 0) {
                if ((u32)g[70] >= 6) {
                    g[70] = 0;
                    *(s16 *)(pb + 0) = (s16)(*(u16 *)(pb + 0) ^ 1);
                } else {
                    g[70] = (u8)(g[70] + 1);
                }
            } else {
                g[70] = 6;
            }
            if ((*(u16 *)(pb + 0) & 2) != 0) {
                if ((u32)g[71] >= 6) {
                    g[71] = 0;
                    *(s16 *)(pb + 0) = (s16)(*(u16 *)(pb + 0) ^ 2);
                } else {
                    g[71] = (u8)(g[71] + 1);
                }
            } else {
                g[71] = 6;
            }
        }
    }

    *(s16 *)(pb + 4) = (s16)(~*(u16 *)(pb + 2) & *(u16 *)(pb + 0));
    w = *(u16 *)(pa + 8);
    pa += 8;
    *(s16 *)(pa + 2) = (s16)w;
    t = *(u16 *)(pp + 2);
    *(s16 *)(pa + 4) = (s16)(~w & t);
    *(s16 *)(pa + 0) = (s16)t;
}
