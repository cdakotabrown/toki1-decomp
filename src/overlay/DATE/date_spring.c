/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=1 */
/* Function: date_spring */
/* Overlay: DATE */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x8c --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0xa4 --offset2 0xb0 */
/* @PostProc: swap_instructions --offset1 0xa8 --offset2 0xb4 */
/* @PostProc: swap_instructions --offset1 0xac --offset2 0xb0 */
/* @PostProc: swap_instructions --offset1 0xb0 --offset2 0xb4 */
/* @PostProc: swap_instructions --offset1 0xc0 --offset2 0xdc */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xc8 */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xf0 */
/* @PostProc: swap_instructions --offset1 0xcc --offset2 0xf8 */
/* @PostProc: swap_instructions --offset1 0xd0 --offset2 0xdc */
/* @PostProc: swap_instructions --offset1 0xd4 --offset2 0xf0 */
/* @PostProc: swap_instructions --offset1 0xdc --offset2 0xe8 */
/* @PostProc: swap_instructions --offset1 0xe0 --offset2 0xe8 */
/* @PostProc: swap_instructions --offset1 0xe4 --offset2 0xf0 */
/* @PostProc: swap_instructions --offset1 0xec --offset2 0xf0 */
/* @PostProc: swap_instructions --offset1 0xf0 --offset2 0x108 */
/* @PostProc: swap_instructions --offset1 0xf4 --offset2 0xf8 */
/* @PostProc: swap_instructions --offset1 0xf8 --offset2 0x108 */
/* @PostProc: swap_instructions --offset1 0xfc --offset2 0x108 */
/* @PostProc: swap_instructions --offset1 0x100 --offset2 0x108 */
/* @PostProc: patch_insn_reg_field 0x14:rt:v0:a0 0x18:rs:v0:a0 0x18:rt:v0:a0 0x44:rs:v0:a0 0x58:rs:v0:a0 0x84:rt:s5:s7 0x88:rt:s6:s5 0x8c:rs:s6:s5 0x8c:rt:s6:s5 0x90:rs:s5:s7 0x90:rt:s5:s7 0x9c:rt:s7:s6 0xa0:rt:s7:s6 0xa4:rt:a0:t7 0xa8:rs:a0:t7 0xa8:rt:a0:t7 0xac:rd:t6:t0 0xb0:rs:s5:s7 0xb0:rt:t6:t0 0xb4:rt:a1:t2 0xb8:rd:t7:t6 0xbc:rs:s6:s5 0xbc:rt:t7:t6 0xd0:rt:a2:t9 0xd4:rt:a3:t8 0xd8:rs:a1:t2 0xdc:rd:v1:t0 0xdc:rs:t4:t8 0xdc:rt:t0:t9 0xe0:rt:t0:t1 0xe4:rt:t2:t0 0xe8:rt:t1:t2 0xf0:rt:a0:t7 0xf4:rd:t2:t5 0xf4:rs:a3:t1 0xf4:rt:a2:t4 0xf8:rd:t5:t7 0xf8:rt:v1:t5 0xfc:rs:t1:t2 0x100:rt:v1:t5 0x108:rt:t5:t7 0x128:rd:t7:t1 0x12c:rd:t8:t4 0x12c:rt:t7:t1 0x130:rd:t9:t5 0x130:rt:t8:t4 0x134:rs:t9:t5 0x134:rt:t2:t6 0x138:rt:t3:t7 0x13c:rs:t3:t7 0x13c:rt:t2:t6 0x144:rd:t6:t3 0x148:rs:t6:t3 0x14c:rs:t2:t6 0x15c:rs:t2:t6 0x164:rs:t3:t7 0x188:rd:t4:t8 0x18c:rs:t4:t8 0x18c:rt:t5:t9 0x190:rt:t5:t9 0x194:rt:t6:t0 0x19c:rt:t6:t0 0x1b8:rt:t7:t2 0x1bc:rs:t7:t2 0x1c4:rs:t7:t2 0x1c4:rt:t7:t2 0x1c8:rd:t8:t3 0x1c8:rt:t7:t2 0x1cc:rs:t8:t3 0x1cc:rt:t9:t1 0x1d4:rt:t3:t4 0x1d8:rs:t3:t4 0x1e0:rs:t3:t4 0x1e0:rt:t3:t4 0x1e4:rs:t9:t1 0x1e4:rt:t3:t4 0x1e8:rt:t8:t0 0x1ec:rd:t8:t0 0x1ec:rt:t8:t0 0x1f0:rd:t9:t2 0x1f0:rt:t8:t0 0x1f4:rt:t6:t8 0x1f8:rd:t2:t5 0x1fc:rs:t2:t5 0x1fc:rt:t4:t7 0x204:rs:t4:t7 0x220:rs:t4:t7 0x230:rs:t9:t2 0x234:rd:t5:t6 0x238:rd:t7:t9 0x238:rs:t6:t8 0x238:rt:t5:t6 0x23c:rt:t7:t9 0x268:rd:t2:t1 0x26c:rt:t2:t1 */
#include "types.h"

extern u8 date_pattern;
extern u8 now_month;
extern volatile u8 fade_bri;
extern u8 class_w[][68];
extern s32 date_spring_pos[];

extern s32 rand(void);

s32 date_spring(void)
{
    u8 *spr;
    s32 *slot;
    s16 i;
    s16 r;
    s32 b;
    s32 a;
    s32 x;
    s32 y;
    s32 vx;
    s32 vy;
    s32 nvx;
    s32 c;
    s32 denom;
    s32 mode;
    s32 g;
    s32 h;

    if (date_pattern == 1) {
        return 0;
    }
    if (date_pattern != 2) {
        if (now_month < 9U) {
            goto ret0;
        }
        if (now_month < 12U) {
            goto run;
        }
ret0:
        return 0;
    }

run:
    for (i = 0; i < 20; i++) {
        g = fade_bri;
        slot = &date_spring_pos[i];
        a = slot[0];
        spr = class_w[i];
        spr[6667] |= 0x80;
        x = *(s32 *)(spr + 6700);
        vy = *(s32 *)(spr + 6712);
        y = *(s32 *)(spr + 6704);
        vx = *(s32 *)(spr + 6708);
        *(s32 *)(spr + 6704) = y + vy;
        h = *(s16 *)(spr + 6706);
        spr[6671] = g;
        nvx = vx + ((a - x) >> 8);
        *(s32 *)(spr + 6708) = nvx;
        *(s32 *)(spr + 6700) = x + nvx;
        if (h >= 41) {
            *(s16 *)(spr + 6706) = -120;
            r = rand() % 2;
            *(s16 *)(spr + 6690) = 4096 / (r * 2 + 2);
            *(s16 *)(spr + 6692) = 4096 / (r * 2 + 2);
            *(s16 *)(spr + 6702) = rand() % 300 - 150;
            slot[0] = *(s32 *)(spr + 6700);
            b = rand();
            c = rand();
            denom = r + 1;
            mode = date_pattern;
            *((s16 *)slot + 1) = *((s16 *)slot + 1)
                + (((c % 2) * 2 - 1) * (b % 8) + 8) / denom;
            *(s32 *)(spr + 6712) = (((mode != 2) << 15) + 32768) / denom;
        }
    }
}
