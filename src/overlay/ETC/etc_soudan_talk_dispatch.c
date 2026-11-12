/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x8014B6D0 */
/* @SymbolSize: 84 */
/* Address: 0x8014B6D8, Size: 84 bytes (21 insns w/ 2-insn cross-prologue tail) */
#include "types.h"

extern s32 etc_soudan_main1_0(void);
extern s32 etc_soudan_main1_1(void);

extern u8 now_sub_sub_step;

s32 etc_soudan_talk_dispatch(void)
{
    s32 ret;

    ret = now_sub_sub_step;
    switch (now_sub_sub_step) {
    case 0:
        ret = etc_soudan_main1_0();
        break;
    case 1:
        ret = etc_soudan_main1_1();
        break;
    }
    return ret;
}
