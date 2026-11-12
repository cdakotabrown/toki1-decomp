/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x8014983C */
/* @SymbolSize: 108 */
/* Address: 0x80149844, Size: 108 bytes (27 insns w/ 2-insn cross-prologue tail) */
#include "types.h"

extern s32 etc_siken_init(void);
extern s32 etc_taku_step(void);
extern s32 etc_menu_step(void);

extern u8 now_sub_sub_step;

s32 etc_step0(void)
{
    s32 ret;

    ret = now_sub_sub_step;
    switch (now_sub_sub_step) {
    case 0:
        ret = etc_siken_init();
        break;
    case 1:
        ret = etc_taku_step();
        break;
    case 2:
        ret = etc_menu_step();
        break;
    }
    return ret;
}
