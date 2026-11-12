/* @SymbolStart: 0x80139ECC */
/* @SymbolSize: 72 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void strcpy(void *, void *);
extern s16 shougatu_date_now_serifu;
extern s16 shougatu_date_now_serifu_in;
extern u8 ba_string;
extern u8 shougatu_girl1_basyo_set2_str0;

void shougatu_girl1_basyo_set2(void)
{
    shougatu_date_now_serifu = 3;
    shougatu_date_now_serifu_in = 0;
    strcpy(&ba_string, &shougatu_girl1_basyo_set2_str0);
    sub_sub_step_up();
}
