/* @SymbolStart: 0x80136CC8 */
/* @SymbolSize: 56 */
#include "types.h"

extern void shougatu_serifu_set(void);
extern void sub_sub_step_up(void);
extern s16 now_serifu;
extern s16 now_serifu_in;

void shougatu_serifu_init(void)
{
    now_serifu = 1;
    now_serifu_in = 0;
    shougatu_serifu_set();
    sub_sub_step_up();
}
