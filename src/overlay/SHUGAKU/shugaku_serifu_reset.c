/* shugaku_serifu_reset - SHUGAKU overlay, Address: 0x80137918, Size: 36 bytes (9 insns) */
#include "types.h"

extern s16 now_serifu_shugaku;
extern void sub_sub_step_up(void);

void shugaku_serifu_reset(void)
{
    now_serifu_shugaku = 0;
    sub_sub_step_up();
}
