/* @SymbolStart: 0x80134030 */
/* @SymbolSize: 180 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:v0 0x14:rs:v1:v0 */
/* omimai_serifu_next - OMIMAI overlay, Address: 0x80134038, Size: 172 bytes (43 insns) */
#include "types.h"

extern s16 now_serifu;
extern u8 now_girl;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern u8 omimai_serifu_next_no;
extern s32 omimai_serifu3_res_tbl[][13];
extern void sub_sub_step_up(void);

s32 omimai_serifu_next(void)
{
    u8 v0_hidden = now_girl;

    if (v0_hidden == 9) {
        if (now_serifu == 2) {
            sub_sub_step_up();
            return 0;
        }
    }
    if (v0_hidden == 8) {
        sub_sub_step_up();
        sub_sub_step_up();
        return 0;
    }
    ss_serifu = omimai_serifu3_res_tbl[0][11];
    ss_serifu_kind = omimai_serifu3_res_tbl[1][11];
    sss_sound = omimai_serifu3_res_tbl[2][11];
    now_serifu = (s16)(omimai_serifu_next_no + 1);
    sub_sub_step_up();
    return;
}
