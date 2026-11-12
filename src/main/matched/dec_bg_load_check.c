/* Address: 0x8007EE44 */
/* Size: 132 bytes (33 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x8007EE3C */
/* @SymbolSize: 140 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: patch_insn_reg_field --patch 0x10:rt:v0:t7 --patch 0x18:rs:v0:t7 --patch 0x18:rd:t7:t8 --patch 0x20:rs:t7:t8 --patch 0x28:rt:t8:t9 --patch 0x30:rs:t8:t9 --patch 0x30:rd:t9:t0 --patch 0x38:rs:t9:t0 --patch 0x50:rt:t0:t1 --patch 0x54:rs:t0:t1 --patch 0x54:rt:t0:t1 --patch 0x5c:rs:t0:t1 --patch 0x5c:rt:t1:t2 --patch 0x60:rt:t1:t2 */
#include "types.h"

extern u32 *mdec_bs;
extern s32 date_effect_init_tbl0;
extern s32 date_effect_init_tbl1;
extern u8 now_sub_sub_step;
extern void dec_bg_init(s32);
extern void sub_sub_step_up(void);

s32 dec_bg_load_check(void) {
    u32 *ptr = mdec_bs;
    u32 field0 = *ptr;

    if ((field0 & 0xFFFF0000) != 0x38000000 || (ptr[1] & 0xFF010000) != 0x00010000) {
        date_effect_init_tbl0 = 0;
        date_effect_init_tbl1 = 0;
        sub_sub_step_up();
        now_sub_sub_step -= 3;
        return 0;
    }
    dec_bg_init(1);
    sub_sub_step_up();
}
