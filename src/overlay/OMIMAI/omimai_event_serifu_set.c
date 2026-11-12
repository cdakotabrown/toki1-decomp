/* omimai_event_serifu_set - OMIMAI overlay, Address: 0x80132DF8, Size: 128 bytes (32 insns) */
#include "types.h"

extern u8 day_flag;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 omimai_serifu_res_tbl[][12];
extern void event_log_add(s32, s32);
extern void sub_sub_step_up(void);

void omimai_event_serifu_set(void)
{
    event_log_add(13, 37);
    day_flag = day_flag | 4;
    ss_serifu = omimai_serifu_res_tbl[0][11];
    ss_serifu_kind = omimai_serifu_res_tbl[1][11];
    sss_sound = omimai_serifu_res_tbl[2][11];
    now_serifu = 2;
    now_serifu_in = 0;
    sub_sub_step_up();
}
