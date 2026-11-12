/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x8014A1D8 */
/* @SymbolSize: 108 */
/* Address: 0x8014A1E0, Size: 100 bytes (25 insns) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 etc_siken_cw_judge(void);
extern s32 etc_siken_cw_wait(void);

extern u8 step_mode;

s32 etc_siken_cw_main(void)
{
    s32 ret;

    ret = step_mode;
    switch (step_mode) {
    case 0:
        ret = etc_siken_cw_judge();
        break;
    case 1:
        ret = etc_siken_cw_wait();
        break;
    case 2:
        ret = sub_sub_step_up();
        break;
    }
    return ret;
}
