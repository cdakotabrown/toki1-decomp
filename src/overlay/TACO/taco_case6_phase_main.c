/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x801375A0 */
/* @SymbolSize: 124 */
#include "types.h"

extern u8 now_sub_sub_step;

extern s32 frame_counter;
extern char taco_main6_str0[];

extern s32 taco_phase_init();
extern s32 taco_main6_main1();
extern s32 taco_str_disp2();

s32 taco_case6_phase_main()
{
    s32 ret;

    ret = now_sub_sub_step;
    switch (now_sub_sub_step) {
    case 0:
        ret = taco_phase_init();
        break;
    case 1:
        ret = taco_main6_main1();
        break;
    }
    if ((frame_counter & 0x10) != 0) {
        ret = taco_str_disp2(taco_main6_str0, -64, 0, 2);
    }
    return ret;
}
