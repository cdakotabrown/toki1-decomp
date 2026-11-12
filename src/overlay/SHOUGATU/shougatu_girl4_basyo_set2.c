/* @SymbolStart: 0x8013ADC0 */
/* @SymbolSize: 72 */
#include "types.h"

extern void strcpy(s32 *, s32 *);
extern void sub_sub_step_up(void);
extern s16 shougatu_date_now_serifu;
extern s16 shougatu_date_now_serifu_in;
extern s32 ba_string;
extern s32 shougatu_girl4_basyo_set2_str0;

void shougatu_girl4_basyo_set2(void)
{
    shougatu_date_now_serifu = 12;
    shougatu_date_now_serifu_in = 0;
    strcpy(&ba_string, &shougatu_girl4_basyo_set2_str0);
    sub_sub_step_up();
}
