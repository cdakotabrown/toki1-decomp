/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x8013A3A8 */
/* @SymbolSize: 132 */
#include "types.h"

extern u8 now_sub_sub_step;

extern s32 tel_rusuban_init(void);
extern s32 tel_tatebayasi_dialog(void);
extern s32 tel_tatebayasi_cnt_up(void);
extern s32 tel_goto_step31(void);

s32 tel_tatebayasi_main(void)
{
    s32 ret;

    ret = now_sub_sub_step;
    switch (now_sub_sub_step) {
    case 0:
        ret = tel_rusuban_init();
        break;
    case 1:
        ret = tel_tatebayasi_dialog();
        break;
    case 2:
        ret = tel_tatebayasi_cnt_up();
        break;
    case 3:
        ret = tel_goto_step31();
        break;
    }
    return ret;
}
