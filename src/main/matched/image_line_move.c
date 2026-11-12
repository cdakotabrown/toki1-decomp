/* Address: 0x8004490C - 0x8004495C (80 bytes) */
/* Region: GCC (0x80041000 - 0x80080FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern void MoveImage(RECT *rect, s32 param2, s32 param3);

void image_line_move(s32 arg0, s32 arg1, s32 arg2) {

    RECT rect;
    s32 pad[6];

    (void)pad;

    rect.x = (short)(arg1 << 4);
    rect.y = (short)(arg0 + 480);
    rect.w = 16;
    rect.h = 1;

    MoveImage(&rect, 256, arg2 + 480);
}
