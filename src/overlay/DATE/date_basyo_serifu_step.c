/* @SymbolSize: 144 */
/* Function: date_basyo_serifu_step */
/* Overlay: DATE */
#include "types.h"

extern void basyo_string_step(void);

extern u8 now_basyo;
extern u8 date_pattern;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern u8 date_serifu_stride;

void date_basyo_serifu_step(void)
{
    basyo_string_step();

    if (now_basyo == 11 && date_pattern == 2) {
        now_serifu += date_serifu_stride;
    } else {
        now_serifu += date_pattern * date_serifu_stride;
    }
    now_serifu_in = 0;
}
