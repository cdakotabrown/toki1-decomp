/* Address: 0x80059B30 */
/* Size: 88 bytes (22 instructions) */
/* Region: Game */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u16 libapi_mouse_sense_x;
extern u16 libapi_mouse_sense_y;
extern u32 mouse_x_min;
extern u32 mouse_x_max;
extern u32 mouse_y_min;
extern u32 mouse_y_max;

void RangeMouse(u32 a0, u32 a1, u32 a2, u32 a3) {
    u32 sx = libapi_mouse_sense_x;
    u32 sy = libapi_mouse_sense_y;
    mouse_x_min = a0 * sx;
    mouse_x_max = a1 * sx;
    mouse_y_min = a2 * sy;
    mouse_y_max = a3 * sy;
}
