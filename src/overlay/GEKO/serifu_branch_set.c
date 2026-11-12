/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
/* serifu_branch_set -- GEKO overlay, Address: 0x80133598, Size: 452 bytes (113 insns) */
#include "types.h"

extern void sub_step_up(void);
extern void sub_sub_step_up(void);
extern void device_cmd_send(s32 a0);
extern s32 get_g_zyotai_s(s32 a0);

extern volatile u8 now_girl;
extern s32 geko_ss_serifu;
extern s32 geko_ss_serifu_kind;
extern s32 geko_sss_sound;
extern s16 geko_now_serifu;
extern u8 geko_serifu_state;
extern u8 geko_param_up_flag;
extern u8 geko_stat_hi;

extern u8 girl0_syoushin[];
extern u8 girl0_flag[];
extern s32 geko_serifu_tbl[];
extern s32 geko_serifu_kind_tbl[];
extern s32 geko_sound_tbl[];

void serifu_branch_set(void)
{
    s32 masked7f;
    u8 mB;
    u8 mA;

    masked7f = get_g_zyotai_s(now_girl) & 0x7F;
    mA = masked7f;
    mB = masked7f;

    if (mA == 4 || geko_stat_hi != 0 ||
        (now_girl == 6 && (u32)mA < 2U)) {
        device_cmd_send(36);
        device_cmd_send(1287);
    }

    if (geko_param_up_flag != 0) {
        if (geko_serifu_state == 0 ||
            ((*(s32 *)&girl0_flag[now_girl * 56] & 1) != 0)) {
            sub_sub_step_up();
            return;
        }
    }

    if (geko_param_up_flag == 0 && now_girl == 6 && (u32)mB < 2U) {
        sub_sub_step_up();
        return;
    }

    if (geko_param_up_flag == 0) {
        if ((u32)mB < 2U && geko_stat_hi == 0) {
            geko_ss_serifu = geko_serifu_tbl[10];
            geko_ss_serifu_kind = geko_serifu_kind_tbl[10];
            geko_sss_sound = geko_sound_tbl[10];
            geko_now_serifu = *(s16 *)&girl0_syoushin[now_girl * 56] / 20 + 1;
            if (geko_now_serifu >= 6) {
                geko_now_serifu = 5;
            }
        }
        sub_sub_step_up();
        return;
    }
    sub_step_up();
}
