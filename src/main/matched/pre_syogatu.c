/* Address: 0x80061864, Size: 84 bytes (21 instructions) */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern u8 now_sub_sub_step;
extern s32 pre_syogatu0(void);
extern s32 pre_syogatu1(void);

s32 pre_syogatu(void) {
    s32 mode;
    s32 ret;

    mode = now_sub_sub_step;
    ret = mode;
    switch (mode) {
    case 0:
        ret = pre_syogatu0();
        break;
    case 1:
        ret = pre_syogatu1();
        break;
    }
    return ret;
}
