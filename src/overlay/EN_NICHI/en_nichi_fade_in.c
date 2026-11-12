/* Address: 0x80132CBC, Size: 268 bytes (67 insns) */
/* @SymbolSize: 268 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=128:-1:2:2 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x8c --offset2 0x94 */
/* @PostProc: swap_instructions --offset1 0x8c --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xcc */
/* @PostProc: patch_insn_reg_field 0x94:rt:v1:v0 0x98:rs:v1:v0 0x98:rt:v1:v0 0xa0:rt:t9:t1 0xa4:rt:t1:t3 0xa8:rt:t3:t5 0xac:rt:t5:t9 0xb4:rs:t9:t1 0xb4:rt:t0:t2 0xb8:rs:t1:t3 0xb8:rt:t2:t4 0xbc:rs:t3:t5 0xbc:rt:t4:t6 0xc0:rs:t5:t9 0xc0:rt:t6:t0 0xc4:rt:t4:t6 0xc8:rt:t2:t4 0xcc:rt:t0:t2 0xd0:rt:v1:v0 0xd4:rt:t6:t0 */
#include "types.h"

extern volatile s32 en_nichi_grid_bright;
extern u8 class_w[][68];
extern volatile u8 back_clear_r;
extern volatile u8 back_clear_g;
extern volatile u8 back_clear_b;

void en_nichi_fade_in(void)
{
    s32 v0;
    u8 *p;
    u8 *end;

    v0 = en_nichi_grid_bright;
    if (v0 != 124) {
        goto step;
    }

    v0 = 128;
    en_nichi_grid_bright = v0;
    end = (u8 *)class_w[64];
    p = (u8 *)class_w;
    do {
        p += 272;
        p[6331] = (u8)v0;
        p[6399] = (u8)v0;
        p[6467] = (u8)v0;
        p[6263] = (u8)v0;
    } while (p != end);

    back_clear_r = 64;
    back_clear_g = 64;
    back_clear_b = 128;
    return;

step:
    if (v0 >= 128) {
        return;
    }
    v0 += 4;
    en_nichi_grid_bright = v0;
    end = (u8 *)class_w[64];
    p = (u8 *)class_w;
    do {
        p += 272;
        p[6331] = p[6331] + 4;
        p[6399] = p[6399] + 4;
        p[6467] = p[6467] + 4;
        p[6263] = p[6263] + 4;
    } while (p != end);

    back_clear_r = back_clear_r + 2;
    back_clear_g = back_clear_g + 2;
    back_clear_b = back_clear_b + 4;
}
