/* @SymbolStart: 0x8013BA1C */
/* @SymbolSize: 368 */
/* Function: date_basyo15_serifu_run */
/* Overlay: DATE */
#include "types.h"

extern s32 serifu_main(s32, s32, s32);
extern void device_cmd_send(s32);
extern s32 sub_sub_step_up(void);
extern s32 get_CD_l_com(void);
extern s32 check_end_k(void);
extern void k_reset(s32);

extern u8 step_mode;
extern u8 date_zoo_season;
extern s16 date_now_serifu_basyo15_in;
extern s16 date_now_serifu_basyo15;
extern s32 now_s;
extern s32 now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;
extern s32 date_serifu0_tbl;
extern s32 date_serifu0_kind_tbl;
extern s32 date_sound0_tbl;
extern u8 serifu_no;
extern s32 k_wait_cnt;
extern s32 serifu_voice;
extern s32 serifu_state;
extern u8 yazirushi_cw_disp;
extern s16 sound_seek_time;

s32 date_basyo15_serifu_run(void)
{
    if (step_mode++ < 128U) {
        return 0;
    }
    if (step_mode >= 151U) {
        step_mode = 150;
    }

    date_now_serifu_basyo15_in = date_zoo_season;
    now_s = (s32)&date_now_serifu_basyo15;
    now_s_in = (s32)&date_now_serifu_basyo15_in;
    ss_s = date_serifu0_tbl;
    ss_s_kind = date_serifu0_kind_tbl;
    sss_so = date_sound0_tbl;

    if (serifu_main(serifu_no, 0, 0) != 0) {
        device_cmd_send(196);
        device_cmd_send(512);
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
    device_cmd_send(512);
    serifu_state = 0;
    k_reset(1);
    serifu_voice = 0;
    yazirushi_cw_disp &= 0xFF7F;
    sound_seek_time = 0;
    return sub_sub_step_up();
}
