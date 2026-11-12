/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x8014BAB4 */
/* @SymbolSize: 96 */
/* Address: 0x8014BABC, Size: 96 bytes (24 insns w/ 2-insn cross-prologue tail) */
#include "types.h"

extern s32 xa_wait(void);
extern s32 goto_sub_step(s32 a0);
extern u8 step_mode;

s32 etc_soudan_girl_wait(void)
{
    s32 ret;

    ret = step_mode;
    switch (step_mode) {
    case 0:
        ret = xa_wait();
        break;
    case 1:
        ret = goto_sub_step(2);
        step_mode = 2;
        break;
    }
    return ret;
}
