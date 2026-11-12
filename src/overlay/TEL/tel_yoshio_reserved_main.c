/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x8013A33C */
/* tel_yoshio_reserved_main - TEL overlay, Address: 0x8013A344, Size: 108 bytes (27 insns) */
/* @SymbolSize: 108 */
#include "types.h"

extern u8 now_sub_sub_step;

extern s32 tel_home_ring_init(void);
extern s32 tel_yoshio_reserved_dialog(void);
extern s32 tel_goto_step31(void);

s32 tel_yoshio_reserved_main(void)
{
    s32 ret;

    ret = now_sub_sub_step;
    switch (now_sub_sub_step) {
        case 0:
            ret = tel_home_ring_init();
            break;
        case 1:
            ret = tel_yoshio_reserved_dialog();
            break;
        case 2:
            ret = tel_goto_step31();
            break;
    }
    return ret;
}
