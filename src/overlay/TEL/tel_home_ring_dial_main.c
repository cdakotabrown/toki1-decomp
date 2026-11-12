/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x8013A6CC */
/* @SymbolSize: 132 */
#include "types.h"

extern u8 now_sub_sub_step;

extern s32 tel_home_ring_init(void);
extern s32 tel_dial_step(void);
extern s32 tel_ring_timeout(void);
extern s32 tel_goto_step31(void);

s32 tel_home_ring_dial_main(void)
{
    s32 ret;

    ret = now_sub_sub_step;
    switch (now_sub_sub_step) {
    case 0:
        ret = tel_home_ring_init();
        break;
    case 1:
        ret = tel_dial_step();
        break;
    case 2:
        ret = tel_ring_timeout();
        break;
    case 3:
        ret = tel_goto_step31();
        break;
    }
    return ret;
}
