/* @SymbolStart: 0x80139070 */
/* @SymbolSize: 84 */
/* Function: date_basyo9_serifu_skip */
/* Overlay: DATE */
/* Address: 0x80139070 (cross-prologue head) / 0x80139078 (body) */
/* Size: 84 bytes (21 instructions) */
#include "types.h"

extern u8 date_late96_flag;
extern s16 now_serifu;
extern s32 serifu_no;

extern void sub_sub_step_up(void);

void date_basyo9_serifu_skip(void)
{
    if (date_late96_flag == 0) {
        now_serifu = now_serifu + 1;
        sub_sub_step_up();
        sub_sub_step_up();
        serifu_no = 0;
    }
    sub_sub_step_up();
}
