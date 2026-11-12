/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x8014ADA0 */
/* @SymbolSize: 108 */
/* Address: 0x8014ADA8, Size: 108 bytes (27 insns w/ 2-insn cross-prologue tail) */
#include "types.h"

extern s32 etc_init0(void);
extern s32 etc_cd_wait(void);
extern s32 etc_soudan_basyo_init(void);

extern u8 now_sub_sub_step;

s32 etc_soudan_setup_dispatch(void)
{
    s32 ret;

    ret = now_sub_sub_step;
    switch (now_sub_sub_step) {
    case 0:
        ret = etc_init0();
        break;
    case 1:
        ret = etc_cd_wait();
        break;
    case 2:
        ret = etc_soudan_basyo_init();
        break;
    }
    return ret;
}
