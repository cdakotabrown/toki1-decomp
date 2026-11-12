/* @SymbolStart: 0x801349F0 */
/* @SymbolSize: 200 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x10:rt:t0:a2 0x14:rs:t0:a2 0x14:rt:t0:a2 0x1c:rs:t0:a2 0x1c:rt:t0:a2 0x20:rt:t0:a2 0x24:rt:a3:a0 0x2c:rd:t0:a2 0x30:rs:a3:a0 0x30:rt:a3:a0 0x38:rt:a2:a1 0x3c:rt:a2:a1 0x44:rd:t8:t1 0x48:rt:t8:t1 0x50:rd:a1:t8 0x54:rd:a0:v0 0x54:rs:a3:a0 0x54:rt:a1:t8 0x58:rs:a0:v0 0x58:rt:v0:t9 0x60:rs:v0:t9 0x60:rt:t2:t0 0x68:rs:a0:v0 0x68:rt:t2:t0 0x70:rt:t0:a2 0x74:rs:t0:a2 */
/* ending_cal_scroll - ENDING overlay, Address: 0x801349F0, Size: 200 bytes (50 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern u8 Sprites[];
extern u8 cal_cw_mode;
extern s16 cal_cw_tick;
extern s16 cal_cw_anim;
extern s16 cal_cw_frame;
extern s16 cal_cw_x;
extern u8 cw97_mode;
extern u8 cw97_disp;

void ending_cal_scroll(void)
{
    s16 a2;
    s32 v1;
    u8 *base;
    s32 stride;
    u8 *entry;
    s16 cur;
    s32 t8;

    a2 = (s16)(cal_cw_x - 1);
    base = Sprites;
    v1 = 0;
    stride = 36;
    do {
        t8 = v1 * stride;
        v1++;
        v1 = (s16)v1;
        entry = base + t8;
        cur = *(s16 *)(entry + 148);
        cur--;
        *(s16 *)(entry + 148) = cur;
    } while (v1 < 6);
    cal_cw_x = a2;
    if (a2 < -158) {
        cal_cw_mode = 1;
        cal_cw_anim = 1;
        cal_cw_frame = 0;
        cal_cw_tick = 0;
        cw97_disp = (u8)(cw97_disp | 0x80);
        cw97_mode = 5;
        sub_sub_step_up();
    }
}
