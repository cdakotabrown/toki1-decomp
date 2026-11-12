/* Address: 0x80059CB8 */
/* Size: 232 bytes (58 instructions, last two are alignment nops) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=6:t2,7:t3,8:t4,9:t5 IDO52_PRECOLOR_WEB=v0:4,a0:16 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s8 *mouse_rec_tbl[];
extern s32 mouse_x_acc[];
extern s32 mouse_y_acc[];
extern s32 mouse_x_min;
extern s32 mouse_x_max;
extern s32 mouse_y_min;
extern s32 mouse_y_max;

void mouse_pos_clamp(s32 idx) {
    s8 *rec = mouse_rec_tbl[idx];
    s32 x;

    if (rec[0] != 0) return;
    if (rec[1] != 18) return;

    x = rec[4] + mouse_x_acc[idx];
    mouse_x_acc[idx] = x;
    mouse_y_acc[idx] = rec[5] + mouse_y_acc[idx];

    if (mouse_x_max < x) {
        mouse_x_acc[idx] = mouse_x_max;
    } else if (x < mouse_x_min) {
        mouse_x_acc[idx] = mouse_x_min;
    }
    if (mouse_y_max < mouse_y_acc[idx]) {
        mouse_y_acc[idx] = mouse_y_max;
        return;
    }
    if (mouse_y_acc[idx] < mouse_y_min) {
        mouse_y_acc[idx] = mouse_y_min;
    }
}
