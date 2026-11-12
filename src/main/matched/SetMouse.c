/* Address: 0x80059B9C */
/* Size: 68 bytes (to the next known symbol at 0x80059BE0) */
/* @PostProc: strip_leading_instructions --count 4 */
/* Address: 0x80059B9C, Size: 68 bytes */
#include "types.h"
extern s16 libapi_mouse_sense_x;
extern s16 libapi_mouse_sense_y;
extern u32 mouse_x_acc[];
extern u32 mouse_y_acc[];

void SetMouse(s32 idx, u32 a1, u32 a2) {
    u32 f1 = (u16)libapi_mouse_sense_x;
    u32 f2 = (u16)libapi_mouse_sense_y;
    mouse_x_acc[idx] = f1 * a1;
    mouse_y_acc[idx] = f2 * a2;
}
