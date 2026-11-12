/* @SymbolStart: 0x80135BDC */
/* @SymbolSize: 128 */
/* date_skating_step - DATE overlay, Address: 0x80135C08, Size: 96 bytes (24 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void dec_bg_cache_step(s32);
extern void event_log_add(s32, s32);
extern void strcpy(s32, s32);

extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern u8 date_serifu_stride;
extern s32 ba_string;
extern s32 date_skating_step_str0;
extern s32 date_serifu_tbl[];
extern s32 date_serifu_kind_tbl[];
extern s32 date_sound_tbl[];

void date_skating_step(void)
{
    ss_serifu = date_serifu_tbl[14];
    ss_serifu_kind = date_serifu_kind_tbl[14];
    sss_sound = date_sound_tbl[14];
    date_serifu_stride = 0;
    strcpy((s32)&ba_string, (s32)&date_skating_step_str0);
    dec_bg_cache_step(17696);
    event_log_add(1, 2);
    sub_sub_step_up();
}
