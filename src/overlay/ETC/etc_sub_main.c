/* @SymbolStart: 0x80143F48 */
/* @SymbolSize: 156 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=8:v1 */
/* Address: 0x80143F58 (prologue); window starts at the absorbed */
#include "types.h"

extern s32 step_counter;
extern u8 now_sub_sub_step;

extern void etc_xa_start();
extern void etc_roll_photo_cam_main();
extern void etc_fade_exit();
extern void etc_attract();
extern void etc_roll_matrix_set();

void etc_sub_main(void)
{
    step_counter++;
    switch (now_sub_sub_step) {
    case 0:
        etc_xa_start();
        break;
    case 1:
        etc_roll_photo_cam_main();
        break;
    case 2:
        etc_fade_exit();
        break;
    case 255:
        etc_attract();
        break;
    }
    etc_roll_matrix_set();
}
