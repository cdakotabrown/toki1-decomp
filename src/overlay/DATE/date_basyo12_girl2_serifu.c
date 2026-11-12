/* @SymbolStart: 0x8013DA64 */
/* @SymbolSize: 116 */
/* Address: 0x8013DA64 (cross-prologue head) / 0x8013DA6C (body) */
/* Size: 116 bytes (29 instructions) */
#include "types.h"

extern void serifu_run(void);

extern u8 now_girl;
extern volatile s16 now_serifu_in;
extern s32 kuchi_stop;

void date_basyo12_girl2_serifu(void)
{
    if (now_serifu_in == 1) {
        now_girl = 14;
    } else {
        now_girl = 2;
    }
    serifu_run();
    if (now_serifu_in == 2) {
        kuchi_stop = 1;
    } else {
        kuchi_stop = 0;
    }
}
