/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x8014BB14 */
/* @SymbolSize: 108 */
/* Address: 0x8014BB1C, Size: 108 bytes (27 insns w/ 2-insn cross-prologue tail) */
#include "types.h"

extern s32 etc_soudan_girl_taku_step(void);
extern s32 etc_soudan_girl_select(void);
extern s32 etc_soudan_girl_wait(void);

extern u8 now_sub_sub_step;

s32 etc_sub_sub_dispatch(void)
{
    s32 ret;

    ret = now_sub_sub_step;
    switch (now_sub_sub_step) {
    case 0:
        ret = etc_soudan_girl_taku_step();
        break;
    case 1:
        ret = etc_soudan_girl_select();
        break;
    case 2:
        ret = etc_soudan_girl_wait();
        break;
    }
    return ret;
}
