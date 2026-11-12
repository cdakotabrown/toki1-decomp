/* Address: 0x80132DC8, Size: 248 bytes (62 insns) */
/* @SymbolSize: 248 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0xb0 --offset2 0xb8 */
/* @PostProc: patch_insn_reg_field 0x28:rt:v1:v0 0x2c:rs:v1:v0 0x2c:rt:v1:v0 0x48:rt:v1:v0 0x80:rt:v1:v0 0x84:rs:v1:v0 0x84:rt:v1:v0 0x8c:rt:t6:t8 0x90:rt:t8:t0 0x94:rt:t0:t2 0x98:rt:t2:t6 0xa0:rs:t6:t8 0xa0:rt:t7:t9 0xa4:rs:t8:t0 0xa4:rt:t9:t1 0xa8:rs:t0:t2 0xa8:rt:t1:t3 0xac:rs:t2:t6 0xac:rt:t3:t7 0xb0:rt:t1:t3 0xb4:rt:t9:t1 0xb8:rt:t7:t9 0xbc:rt:v1:v0 0xc0:rt:t3:t7 */
#include "types.h"

extern volatile s32 en_nichi_grid_bright;
extern u8 class_w[][68];
extern volatile u8 back_clear_r;
extern volatile u8 back_clear_g;
extern volatile u8 back_clear_b;

void en_nichi_fade_out(void)
{
    s32 v0;
    u8 *p;
    u8 *end;

    v0 = en_nichi_grid_bright;
    if (v0 != 4) {
        goto step;
    }

    en_nichi_grid_bright = 0;
    end = (u8 *)class_w[64];
    p = (u8 *)class_w;
    do {
        p += 272;
        p[6331] = 0;
        p[6399] = 0;
        p[6467] = 0;
        p[6263] = 0;
    } while (p != end);

    back_clear_r = 0;
    back_clear_g = 0;
    back_clear_b = 0;
    return;

step:
    if (v0 <= 0) {
        return;
    }
    v0 -= 4;
    en_nichi_grid_bright = v0;
    end = (u8 *)class_w[64];
    p = (u8 *)class_w;
    do {
        p += 272;
        p[6331] = p[6331] - 4;
        p[6399] = p[6399] - 4;
        p[6467] = p[6467] - 4;
        p[6263] = p[6263] - 4;
    } while (p != end);

    back_clear_r = back_clear_r - 2;
    back_clear_g = back_clear_g - 2;
    back_clear_b = back_clear_b - 4;
}
