/* Address: 0x80060E20, Size: 84 bytes (21 instructions) */
/* @SymbolStart: 0x80060E20 */
/* @SymbolSize: 84 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern u8 now_sub_sub_step;
extern s32 uwasa_main0(void);
extern s32 uwasa_exit0(void);

s32 uwasa0(void) {
    s32 mode;
    s32 ret;

    mode = now_sub_sub_step;
    ret = mode;
    switch (mode) {
    case 0:
        ret = uwasa_main0();
        break;
    case 1:
        ret = uwasa_exit0();
        break;
    }
    return ret;
}
