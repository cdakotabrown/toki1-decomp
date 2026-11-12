/* @SymbolStart: 0x80137F70 */
/* @SymbolSize: 84 */
#include "types.h"

extern void strcpy(s32 *, s32 *);
extern void sub_sub_step_up(void);
extern u8 shougatu_date_kind;
extern s16 shougatu_date_now_serifu;
extern s16 shougatu_date_now_serifu_in;
extern s32 ba_string;
extern s32 shougatu_girl8_basyo_set_str0;

void shougatu_girl8_basyo_set(void)
{
    shougatu_date_now_serifu = shougatu_date_kind * 3;
    shougatu_date_now_serifu_in = 0;
    strcpy(&ba_string, &shougatu_girl8_basyo_set_str0);
    sub_sub_step_up();
}
