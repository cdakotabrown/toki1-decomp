/* @SymbolSize: 136 */
/* Address: 0x80133610 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 strcpy(s32 *a0, s32 *a1);
extern s32 ba_string2;
extern s32 shougatu_serifu6_set_str0;
extern s32 shougatu_meet_ss_serifu;
extern s32 shougatu_meet_ss_serifu_kind;
extern s32 shougatu_meet_sss_sound;
extern s16 shougatu_meet_now_serifu;
extern u8 shougatu_serifu6_flag;
extern u8 shougatu_alone_flag;
extern s16 shougatu_voice_cnt;
extern s32 shougatu_meet_serifu_tbl[];
extern s32 shougatu_meet_serifu_kind_tbl[];
extern s32 shougatu_meet_sound_tbl[];

void shougatu_serifu6_set(void)
{
    strcpy(&ba_string2, &shougatu_serifu6_set_str0);
    shougatu_voice_cnt = 0;
    shougatu_alone_flag = 0;
    shougatu_serifu6_flag = 1;
    shougatu_meet_now_serifu = 6;
    shougatu_meet_ss_serifu = shougatu_meet_serifu_tbl[2];
    shougatu_meet_ss_serifu_kind = shougatu_meet_serifu_kind_tbl[2];
    shougatu_meet_sss_sound = shougatu_meet_sound_tbl[2];
    sub_sub_step_up();
}
