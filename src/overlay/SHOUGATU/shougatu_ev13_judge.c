/* @CompileFlags: -cckr */
/* @SymbolStart: 0x80136BBC */
/* @SymbolSize: 268 */
#include "types.h"

extern s32 get_g_zyotai_s();
extern s32 get_g_zyotai_h();
extern void bustup_face_change();
extern void event_log_add();
extern void sub_sub_step_up();

extern u8 sd_proc_on[];
#define G_EE7E sd_proc_on[0x6EA]
#define G_F6BF sd_proc_on[0xF2B]
extern u8 now_sub_sub_step;
extern u8 shougatu_girl0_good;
extern s32 shougatu_meet_ss_serifu;
extern s32 shougatu_meet_ss_serifu_kind;
extern s32 shougatu_meet_sss_sound;
extern s16 shougatu_meet_now_serifu;
extern s16 shougatu_meet_now_serifu_in;
extern s32 shougatu_meet_serifu_tbl[];
extern s32 shougatu_meet_serifu_kind_tbl[];
extern s32 shougatu_meet_sound_tbl[];

void shougatu_ev13_judge()
{
    G_EE7E |= 8;
    if ((u8)(get_g_zyotai_s(G_F6BF) & 0x7F) >= 2) {
        now_sub_sub_step += 9;
    } else {
        if (G_F6BF == 0 && shougatu_girl0_good == 1) {
            event_log_add(13, 45);
        } else {
            event_log_add(13, 44);
        }
        G_EE7E |= 4;
        shougatu_meet_ss_serifu = shougatu_meet_serifu_tbl[12];
        shougatu_meet_ss_serifu_kind = shougatu_meet_serifu_kind_tbl[12];
        shougatu_meet_sss_sound = shougatu_meet_sound_tbl[12];
        shougatu_meet_now_serifu = 0;
        shougatu_meet_now_serifu_in = 0;
        bustup_face_change(get_g_zyotai_h(G_F6BF) & 0xFF);
        sub_sub_step_up();
    }
}
