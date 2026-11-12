/* @SymbolStart: 0x8013DE34 */
/* @SymbolSize: 80 */
/* Address: 0x8013DE34 (cross-prologue head) / 0x8013DE3C (body) */
/* Size: 80 bytes (20 instructions) */
#include "types.h"

extern s16 now_serifu_in;
extern u8 girl_no;
extern s8 now_girl;
extern s32 kuchi_stop;

extern s32 serifu_run(void);

void date_basyo8_serifu_run(void)
{
    if (now_serifu_in == 1) {
        now_girl = girl_no;
    }
    if (now_serifu_in == 2) {
        kuchi_stop = 0;
    }
    serifu_run();
}
