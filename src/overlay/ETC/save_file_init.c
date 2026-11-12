/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x801449C4 */
/* @SymbolSize: 136 */
/* Address: 0x801449CC, Size: 128 bytes (32 insns) */
#include "types.h"

extern u8 screen_bri;
extern s32 default_data_vram_load(void);
extern s32 boot_data_load(void);
extern s32 save_play_show(void);
extern s32 save_file_ask(void);

extern u8 now_sub_sub_step;

s32 save_file_init(void)
{
    s32 ret;

    ret = now_sub_sub_step;
    switch (now_sub_sub_step) {
    case 0:
        screen_bri = 0;
        ret = boot_data_load();
        break;
    case 1:
        ret = default_data_vram_load();
        break;
    case 2:
        ret = save_play_show();
        break;
    case 3:
        ret = save_file_ask();
        break;
    }
    return ret;
}
