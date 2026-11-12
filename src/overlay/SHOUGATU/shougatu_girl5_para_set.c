/* @SymbolStart: 0x801340AC */
/* Address: 0x801340B4 */
/* Size: 128 bytes (32 instructions) */
/* @SymbolSize: 136 */
#include "types.h"

extern void param_clamp(void);
extern void sub_sub_step_up(void);

extern volatile s16 girl5_tokimeki;
extern volatile s16 girl5_yuukou;
extern volatile s16 girl5_syoushin;
extern s32 serifu_no;

void shougatu_girl5_para_set(void)
{
    if (serifu_no != 0) {
        girl5_yuukou = girl5_yuukou - 1;
        girl5_tokimeki = girl5_tokimeki - 1;
        girl5_syoushin = girl5_syoushin + 10;
    } else {
        girl5_tokimeki = girl5_tokimeki + 1;
    }
    param_clamp();
    sub_sub_step_up();
}
