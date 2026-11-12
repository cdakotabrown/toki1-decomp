/* Address: 0x80059B20 */
/* Size: 16 bytes (to the next known symbol at 0x80059B30) */
/* @PostProc: strip_leading_instructions --count 4 */
/* Address: 0x80059B20, Size: 16 bytes */
#include "types.h"

u32 mouse_rec_tbl[2] = { 0, 0 };

void InitMouse(s32 a, s32 b) {
    mouse_rec_tbl[0] = (u32)a;
    mouse_rec_tbl[1] = (u32)b;
}
