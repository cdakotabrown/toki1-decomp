/* @SymbolStart: 0x80146360 */
/* @SymbolSize: 140 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=8:v1 */
/* Address: 0x80146370 (prologue); window starts at the absorbed */
#include "types.h"

extern s32 step_state;
extern u8 now_sub_step;

extern void bukatu_result_main();
extern void etc_white_light_main();
extern void etc_sub_sub_main2();
extern void game_reset();

void etc_last_photo_main(void)
{
    step_state++;
    switch (now_sub_step) {
    case 0:
        bukatu_result_main();
        break;
    case 1:
        etc_white_light_main();
        break;
    case 2:
        etc_sub_sub_main2();
        break;
    default:
        game_reset();
        break;
    }
}
