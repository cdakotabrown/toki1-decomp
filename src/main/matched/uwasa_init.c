/* Address: 0x80060D5C, Size: 84 bytes (21 instructions) */
/* @SymbolStart: 0x80060D5C */
/* @SymbolSize: 84 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern u8 now_sub_sub_step;
extern s32 uwasa_init0(void);
extern s32 uwasa_init1(void);

s32 uwasa_init(void) {
    s32 mode;
    s32 ret;

    mode = now_sub_sub_step;
    ret = mode;
    switch (mode) {
    case 0:
        ret = uwasa_init0();
        break;
    case 1:
        ret = uwasa_init1();
        break;
    }
    return ret;
}
