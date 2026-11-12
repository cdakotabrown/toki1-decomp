/* @CompileEnv: IDO52_FORCE_WEB_COLOR=0:v1,5:v0 */
/* @SymbolStart: 0x8013842C */
/* @SymbolSize: 92 */
/* Function: date_basyo14_girl14_serifu */
/* Overlay: DATE */
/* Address: 0x8013842C (cross-prologue head) / 0x80138434 (body) */
/* Size: 92 bytes (23 instructions) */
#include "types.h"

extern s32 serifu_run(void);

extern volatile s32 step_counter;
extern volatile u8 now_girl;
extern volatile u8 girl_no;

s32 date_basyo14_girl14_serifu(void)
{
    s32 count;
    s32 lt;

    count = step_counter;
    lt = (u32)count < 1u;
    count = count + 1;
    step_counter = count;
    if (lt) {
        now_girl = 14;
    } else {
        now_girl = girl_no;
    }
    return serifu_run();
}
