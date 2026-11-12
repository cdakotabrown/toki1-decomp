/* @SymbolStart: 0x80136128 */
/* @SymbolSize: 48 */
#include "types.h"

extern void sub_sub_step_up(void);
extern s16 now_serifu;
extern s16 now_serifu_in;

void shougatu_serifu_init2(void)
{
    now_serifu = 2;
    now_serifu_in = 0;
    sub_sub_step_up();
}
