/* @SymbolStart: 0x80133B90 */
/* @SymbolSize: 96 */
#include "types.h"

extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern s32 ending_serifu_tbl[];
extern s32 ending_serifu_kind_tbl[];
extern s32 ending_sound_tbl[];

extern void ending_ptr_init(void);
extern void sub_sub_step_up(void);

void ending_serifu_init(void)
{
    now_serifu = 0;
    now_serifu_in = 0;
    ending_ptr_init();
    ss_serifu = ending_serifu_tbl[13];
    ss_serifu_kind = ending_serifu_kind_tbl[13];
    sss_sound = ending_sound_tbl[13];
    sub_sub_step_up();
}
