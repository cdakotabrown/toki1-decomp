/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x801462DC */
/* @SymbolSize: 132 */
/* Address: 0x801462E4, Size: 140 bytes (35 insns w/ 4-insn cross-prologue tail) */
#include "types.h"

extern s32 etc_last_photo_exit(void);
extern s32 etc_save_exit(void);
extern s32 etc_album_exit(void);
extern s32 etc_title_exit(void);

extern u8 now_sub_sub_step;

s32 etc_sub_sub_main2(void)
{
    s32 ret;

    ret = now_sub_sub_step;
    switch (now_sub_sub_step) {
    case 0:
        ret = etc_last_photo_exit();
        break;
    case 1:
        ret = etc_save_exit();
        break;
    case 2:
        ret = etc_album_exit();
        break;
    case 3:
        ret = etc_title_exit();
        break;
    }
    return ret;
}
