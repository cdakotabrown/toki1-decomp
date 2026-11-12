/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x8014BF98 */
/* @SymbolSize: 84 */
/* Address: 0x8014BFA0, Size: 84 bytes (21 insns w/ 2-insn cross-prologue tail) */
#include "types.h"

extern u8 now_sub_sub_step;

extern s32 etc_soudan_bg_load(void);
extern s32 etc_singaku_step(void);

s32 etc_soudan_singaku_dispatch(void)
{
    s32 ret;

    ret = now_sub_sub_step;
    switch (now_sub_sub_step) {
    case 0:
        ret = etc_soudan_bg_load();
        break;
    case 1:
        ret = etc_singaku_step();
        break;
    }
    return ret;
}
