/* @SymbolStart: 0x801365D4 */
/* @SymbolSize: 176 */
#include "types.h"

extern void shougatu_pack_set(void);
extern void event_log_add(s32, s32);
extern void sub_sub_step_up(void);
extern s32 serifu_no;
extern u8 shougatu_serifu_no_bak;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern u8 now_girl;
extern u8 shougatu_girl0_good;
extern s32 shougatu_pack_serifu_tbl[];
extern s32 shougatu_pack_kind_tbl[];
extern s32 shougatu_pack_sound_tbl[];

void shougatu_event_set(void)
{
    shougatu_serifu_no_bak = (u8)serifu_no;
    now_serifu = 0;
    now_serifu_in = 0;
    shougatu_pack_set();
    ss_serifu = shougatu_pack_serifu_tbl[1];
    ss_serifu_kind = shougatu_pack_kind_tbl[1];
    sss_sound = shougatu_pack_sound_tbl[1];
    if (now_girl == 0 && shougatu_girl0_good == 1) {
        event_log_add(10, 50);
    } else {
        event_log_add(10, 46);
    }
    sub_sub_step_up();
}
