/* @SymbolStart: 0x8013A750 */
/* @SymbolSize: 108 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 */
/* Address: 0x8013A758; the compare window starts 8 bytes earlier at 0x8013A750, */
/* Size: 108 bytes (27 insns) including the absorbed head. */
#include "types.h"

extern u8 now_sub_sub_step;

extern s32 tel_rusuban_init(void);
extern s32 tel_tome_dialog(void);
extern s32 tel_goto_step31(void);

s32 tel_tome_main(void)
{
    s32 sel;
    s32 ret;

    sel = now_sub_sub_step;
    ret = sel;
    switch (sel) {
        case 0:
            ret = tel_rusuban_init();
            break;
        case 1:
            ret = tel_tome_dialog();
            break;
        case 2:
            ret = tel_goto_step31();
            break;
    }
    return ret;
}
