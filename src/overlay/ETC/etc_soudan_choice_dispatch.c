/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x8014B8F8 */
/* @SymbolSize: 84 */
/* Address: 0x8014B900, Size: 84 bytes (21 insns w/ 2-insn cross-prologue tail) */
#include "types.h"

extern s32 etc_soudan_taku_step(void);
extern s32 etc_menu_main(void);

extern u8 now_sub_sub_step;

s32 etc_soudan_choice_dispatch(void)
{
    s32 ret;

    ret = now_sub_sub_step;
    switch (now_sub_sub_step) {
    case 0:
        ret = etc_soudan_taku_step();
        break;
    case 1:
        ret = etc_menu_main();
        break;
    }
    return ret;
}
