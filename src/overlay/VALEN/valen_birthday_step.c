/* @SymbolStart: 0x801336D0 */
/* @SymbolSize: 252 */
/* 1. ⛔ DELETED `@PostProc: scheduler_normalizer`.  It was the whole */
/* @PostProc: swap_instructions --offset1 0xa0 --offset2 0xa4 */
/* @PostProc: patch_insn_reg_field 0x44:rt:a0:v0 0x48:rs:a0:v0 0xc8:rt:a0:v0 0xcc:rs:a0:v0 */
/* @PostProc: swap_instructions --offset1 0xa4 --offset2 0xa8 */
/* @PostProc: swap_instructions --offset1 0x9c --offset2 0xa0 */
/* valen_birthday_step - VALEN overlay, Address: 0x801336E4, Size: 232 bytes (58 insns) */
#include "types.h"

extern s32 p_birthday;
extern u8 now_day;
extern u8 now_month;
extern u8 now_girl;
extern u8 valen_top3_tbl[];
extern u8 now_sub_step;
extern u8 now_sub_sub_step;
extern u8 ret_sub_step;
extern u8 ret_sub_sub_step;
extern s32 get_h_tokimeki(s32);
extern void goto_sub_step(s32);
extern void valen_serifu_run(void);
extern void sub_sub_step_up(void);

s32 valen_birthday_step(void)
{
    s32 charId;
    u32 v0;

    v0 = p_birthday;
    if (now_day == ((v0 << 23) >> 27) &&
        now_month == (v0 & 0xF) &&
        (u32)get_h_tokimeki(now_girl) >= 70) {
        charId = now_girl;
        if (charId == valen_top3_tbl[0] || charId == valen_top3_tbl[1] || charId == valen_top3_tbl[2]) {
            ret_sub_step = now_sub_step;
            ret_sub_sub_step = (u8)(now_sub_sub_step + 1);
            goto_sub_step(3);
            return 0;
        }
    }
    if ((u32)get_h_tokimeki(now_girl) >= 70) {
        valen_serifu_run();
    } else {
        sub_sub_step_up();
    }
}
