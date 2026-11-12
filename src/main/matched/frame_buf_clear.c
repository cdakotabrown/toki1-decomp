/* Address: 0x80059688 */
/* Size: 132 bytes (33 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_instructions 0x10 0x14 */
/* @PostProc: swap_instructions 0x48 0x50 */
#include "types.h"

extern void ClearImage(RECT *rect, int a1, int a2, int a3);
extern void DrawSync(int arg);

void frame_buf_clear(void) {
    RECT rect;
    volatile int _pad[4];

    rect.w = 256;
    rect.h = 480;
    rect.x = 0;
    rect.y = 0;
    ClearImage(&rect, 0, 0, 0);
    DrawSync(0);

    rect.x = 256;
    rect.w = 256;
    rect.h = 480;
    rect.y = 0;
    ClearImage(&rect, 0, 0, 0);
    DrawSync(0);
}
