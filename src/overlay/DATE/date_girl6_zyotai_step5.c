/* @SymbolSize: 80 */
/* @PostProc: patch_insn_reg_field 0x28:rt:t6:t7 0x2c:rs:t6:t7 0x34:rt:t7:t8 0x38:rs:t7:t8 0x38:rt:t7:t8 0x40:rs:t7:t8 0x40:rt:t8:t9 0x44:rt:t8:t9 */
/* Function: date_girl6_zyotai_step5 */
/* Overlay: DATE */
/* Address: 0x801577B4 */
/* Size: 80 bytes (20 instructions) */
#include "types.h"

extern s32 get_g_zyotai_s(s32 arg);
extern void sub_sub_step_up(void);

extern u8 now_girl;
extern u16 now_serifuev;

void date_girl6_zyotai_step5(void)
{
    u32 r;

    r = get_g_zyotai_s(now_girl) & 0x7F;
    if (r >= 2) {
        now_serifuev = now_serifuev + 3;
    }
    sub_sub_step_up();
}
