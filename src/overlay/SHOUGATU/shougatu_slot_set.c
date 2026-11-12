/* @SymbolStart: 0x80136804 */
/* @SymbolSize: 96 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void shougatu_text_set(void);
extern s32 shougatu_serifu_save_ptr[];
extern s16 now_serifu;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;

void shougatu_slot_set(void)
{
    now_serifu = 29;
    ss_serifu = shougatu_serifu_save_ptr[0];
    ss_serifu_kind = shougatu_serifu_save_ptr[1];
    sss_sound = shougatu_serifu_save_ptr[2];
    shougatu_text_set();
    sub_sub_step_up();
}
