/* @SymbolStart: 0x80137724 */
/* @SymbolSize: 52 */
#include "types.h"

extern void sub_sub_step_up(void);
extern u32 serifu_state;
extern u8 ending_wait_cnt;
extern u32 ending_serifu_frame;

void ending_serifu_reset(void)
{
    serifu_state = 0;
    ending_wait_cnt = 0;
    ending_serifu_frame = 0;
    sub_sub_step_up();
}
