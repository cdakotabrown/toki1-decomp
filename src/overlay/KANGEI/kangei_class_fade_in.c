/* @SymbolStart: 0x8013748C */
/* @SymbolSize: 192 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x48 */
/* @PostProc: patch_insn_reg_field   0x34:rt:a2:a0 0x44:rs:a2:a0 0x44:rt:a2:a0 0x4c:rt:a3:a1 0x50:rt:a3:a1 0x58:rd:t3:t5 0x5c:rt:t3:t5 0x68:rs:a2:a0 0x6c:rt:a0:t1 0x70:rt:a1:t3 0x74:rs:a0:t1 0x74:rt:t1:t2 0x78:rs:a1:t3 0x78:rt:t2:t4 0x7c:rt:t1:t2 0x84:rt:t2:t4 0x88:rt:t5:t7 0x90:rt:t5:t7 0x94:rt:v0:t8 0x98:rs:v0:t8 0x98:rt:v0:t8 0x9c:rt:t6:t9 0xa0:rs:v0:t8 0xa8:rt:t7:t0 0xb0:rt:t6:t9 0xbc:rt:t7:t0 */
/* kangei_class_fade_in - KANGEI overlay, Address: 0x8013748C (head) / 0x801374A8 (prologue), */
/* Size: 192 bytes (48 insns). */
#include "types.h"

extern void sub_sub_step_up(void);

extern volatile u8 cal_cw_disp;
extern volatile u8 cal_cw_bright;
extern volatile u8 cal_cw_mode;
extern u8 cw97_bright;

extern u8 class_w[];

void kangei_class_fade_in(void)
{
    u8 counter;
    s16 i;
    u8 *entry;
    u8 b3;
    u8 b7;

    cal_cw_disp = cal_cw_disp | 0x80;
    cal_cw_bright = cal_cw_bright + 8;

    i = 97;
    do {
        entry = class_w + (s32)i * 68;
        b3 = entry[3];
        b7 = entry[7];
        entry[3] = (u8)(b3 | 0x80);
        entry[7] = (u8)(b7 + 8);
        i++;
    } while (i < 116);

    cal_cw_mode = 5;
    counter = cal_cw_bright;
    if ((u32)counter >= 128U) {
        cal_cw_bright = 128;
        cw97_bright = 128;
        sub_sub_step_up();
    }
}
