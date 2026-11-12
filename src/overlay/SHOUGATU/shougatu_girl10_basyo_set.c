/* @SymbolStart: 0x8013B214 */
/* @SymbolSize: 68 */
#include "types.h"

extern void strcpy(s32 *, s32 *);
extern void sub_sub_step_up(void);
extern s16 shougatu_date_now_serifu;
extern s16 shougatu_date_now_serifu_in;
extern s32 ba_string;
extern s32 shougatu_girl10_basyo_set_str0;

void shougatu_girl10_basyo_set(void)
{
    shougatu_date_now_serifu = 0;
    shougatu_date_now_serifu_in = 0;
    strcpy(&ba_string, &shougatu_girl10_basyo_set_str0);
    sub_sub_step_up();
}
