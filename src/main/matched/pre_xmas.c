/* Address: 0x80060FDC */
/* Size: 84 bytes (21 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 step_mode;
extern s32 xa_wait(void);
extern s32 goto_step(s32);

s32 pre_xmas(void) {
    s32 mode;
    s32 ret;

    mode = step_mode;
    ret = mode;
    switch (mode) {
    case 0:
        ret = xa_wait();
        break;
    case 1:
        ret = goto_step(82);
        break;
    }
    return ret;
}
