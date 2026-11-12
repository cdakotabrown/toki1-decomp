/* Address: 0x80139F1C */
/* @SymbolStart: 0x80139F14 */
/* @SymbolSize: 72 */
#include "types.h"

extern s16 shougatu_date_now_serifu;
extern s16 shougatu_date_now_serifu_in;
extern u8 ba_string;
extern u8 shougatu_girl1_basyo_set3_str0;

extern void strcpy(void *, void *);
extern void sub_sub_step_up(void);

void shougatu_girl1_basyo_set3(void)
{
    shougatu_date_now_serifu = 6;
    shougatu_date_now_serifu_in = 0;
    strcpy(&ba_string, &shougatu_girl1_basyo_set3_str0);
    sub_sub_step_up();
}
