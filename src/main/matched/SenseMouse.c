/* Address: 0x80059B88, Size: 20 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern s16 libapi_mouse_sense_x;
extern s16 libapi_mouse_sense_y;
void SenseMouse(s32 a, s32 b) {
    libapi_mouse_sense_x = (s16)a;
    libapi_mouse_sense_y = (s16)b;
}
