/* @SymbolStart: 0x80138A3C */
/* @SymbolSize: 84 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 */
/* Address: 0x80138A44; the compare window starts 8 bytes earlier at */
/* Size: 84 bytes (21 insns) including the absorbed head. */
#include "types.h"

extern u8 step_mode;

extern s32 date2_xa_init(void);
extern s32 date2_xa_play(void);

s32 date2_xa_main(void)
{
    s32 sel;
    s32 ret;

    sel = step_mode;
    ret = sel;
    switch (sel) {
        case 0:
            ret = date2_xa_init();
            break;
        case 1:
            ret = date2_xa_play();
            break;
    }
    return ret;
}
