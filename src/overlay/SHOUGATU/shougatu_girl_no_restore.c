/* @SymbolStart: 0x801347E8 */
/* @SymbolSize: 164 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t7:a0 0x48:rs:t7:a0 0x48:rt:t7:a0 0x50:rs:t7:a0 */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 get_g_zyotai_s(s32);
extern void date_name_set(s32);
extern u8 now_girl;
extern s32 serifu_no;
extern s32 serifu_no_bak;
extern s32 shougatu_meet_ss_serifu;
extern s32 shougatu_meet_ss_serifu_kind;
extern s32 shougatu_meet_sss_sound;
extern u8 shougatu_alone_flag;
extern u8 girl_no;
extern s32 shougatu_meet_serifu_tbl[];
extern s32 shougatu_meet_serifu_kind_tbl[];
extern s32 shougatu_meet_sound_tbl[];

void shougatu_girl_no_restore(void)
{
    s32 idx = girl_no;

    s32 r;

    now_girl = idx;
    date_name_set(idx);
    serifu_no = serifu_no_bak;
    r = get_g_zyotai_s(6);
    if (now_girl == 6 && (u32)(r & 0x7F) < 2 && shougatu_alone_flag == 0) {
        shougatu_meet_ss_serifu = shougatu_meet_serifu_tbl[8];
        shougatu_meet_ss_serifu_kind = shougatu_meet_serifu_kind_tbl[8];
        shougatu_meet_sss_sound = shougatu_meet_sound_tbl[8];
    }
    sub_sub_step_up();
}
