/* @SymbolStart: 0x80133E78 */
/* @SymbolSize: 184 */
/* Function: serifu_default_init */
/* Overlay: DATE */
/* Address: 0x80133EAC */
#include "types.h"

extern void sub_sub_step_up(void);
extern void dec_bg_cache_step(s32);
extern void event_log_add(s32, s32);
extern void strcpy(void *, void *);
extern s32 bustup_on;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern u8 date_pattern;
extern u8 date_serifu_stride;
extern s16 imp[];
extern s32 now_date_spot;
extern u8 ba_string;
extern u8 serifu_default_init_str0;
extern s32 date_serifu_tbl[];
extern s32 date_serifu_kind_tbl[];
extern s32 date_sound_tbl[];

void serifu_default_init(void)
{
    ss_serifu = date_serifu_tbl[7];
    ss_serifu_kind = date_serifu_kind_tbl[7];
    sss_sound = date_sound_tbl[7];
    bustup_on = 0;
    date_pattern = 0;
    strcpy(&ba_string, &serifu_default_init_str0);
    date_serifu_stride = 2;
    imp[0] = 0;
    imp[1] = 0;
    imp[2] = 0;
    now_date_spot = 33;
    dec_bg_cache_step(17112);
    event_log_add(0, 33);
    sub_sub_step_up();
}
