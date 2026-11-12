/* @PostProc: patch_insn_reg_field 0x28:rt:t6:t7 */
/* @PostProc: patch_insn_reg_field 0x2c:rs:t6:t7 */
/* @PostProc: patch_insn_reg_field 0x34:rt:t7:t8 */
/* @PostProc: patch_insn_reg_field 0x38:rs:t7:t8 */
/* @PostProc: patch_insn_reg_field 0x38:rt:t7:t8 */
/* @PostProc: patch_insn_reg_field 0x40:rs:t7:t8 */
/* @PostProc: patch_insn_reg_field 0x40:rt:t8:t9 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t8:t9 */
/* geko_serifuev_zyotai_bonus - GEKO overlay, Address: 0x8013F760, Size: 80 bytes (20 insns) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 get_g_zyotai_s(u8);

extern u16 now_serifuev;
extern u8 now_girl;

void geko_serifuev_zyotai_bonus(void)
{
    u32 v0;
    u32 t7;

    v0 = get_g_zyotai_s(now_girl);
    t7 = v0 & 0x7F;
    if (t7 >= 2) {
        now_serifuev = now_serifuev + 2;
    }
    sub_sub_step_up();
}
