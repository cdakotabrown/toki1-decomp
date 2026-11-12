/* @SymbolStart: 0x8013B8AC */
/* @SymbolSize: 368 */
/* Function: date_basyo14_serifu_run */
/* Overlay: DATE */
#include "types.h"

extern s32 serifu_main(s32, s32, s32);
extern void device_cmd_send(s32);
extern s32 sub_sub_step_up(void);
extern s32 get_CD_l_com(void);
extern s32 check_end_k(void);
extern void k_reset(s32);

extern u8 step_mode;
extern u8 date_parade_season;
extern s16 date_now_serifu_basyo14_in;
extern s16 date_now_serifu_basyo14;
extern s32 now_s;
extern s32 now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;
extern u8 serifu_no;
extern s32 k_wait_cnt;
extern s32 serifu_voice;
extern s32 serifu_state;
extern u8 yazirushi_cw_disp;
extern s16 sound_seek_time;
extern s32 date_serifu0_tbl[];
extern s32 date_serifu0_kind_tbl[];
extern s32 date_sound0_tbl[];

s32 date_basyo14_serifu_run(void)
{
    if (step_mode++ < 128U) {
        return 0;
    }
    if (step_mode >= 151U) {
        step_mode = 150;
    }

    date_now_serifu_basyo14_in = date_parade_season;
    now_s = (s32)&date_now_serifu_basyo14;
    now_s_in = (s32)&date_now_serifu_basyo14_in;
    ss_s = date_serifu0_tbl[3];
    ss_s_kind = date_serifu0_kind_tbl[3];
    sss_so = date_sound0_tbl[3];

    if (serifu_main(serifu_no, 0, 0) != 0) {
        device_cmd_send(196);
        device_cmd_send(513);
        sub_sub_step_up();
        k_wait_cnt = 128;
        return 0;
    }

    serifu_voice = 10;
    if (get_CD_l_com() != 9) {
        return;
    }
    if (check_end_k() == 0) {
        return;
    }
    device_cmd_send(513);
    serifu_state = 0;
    k_reset(1);
    serifu_voice = 0;
    yazirushi_cw_disp &= 0xFF7F;
    sound_seek_time = 0;
    return sub_sub_step_up();
}
