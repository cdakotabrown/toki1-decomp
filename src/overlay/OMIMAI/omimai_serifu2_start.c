/* @SymbolSize: 56 */
/* /* omimai_serifu2_start - OMIMAI overlay, Address: 0x80132E78, Size: 56 bytes (14 insns) */
#include "types.h"

extern s16 now_serifu;
extern s16 now_serifu_in;
extern void omimai_serifu_girl_set(void);
extern void sub_sub_step_up(void);

void omimai_serifu2_start(void)
{
    omimai_serifu_girl_set();
    now_serifu = 1;
    now_serifu_in = 0;
    sub_sub_step_up();
}
