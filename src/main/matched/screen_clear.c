/* Address: 0x800412DC */
/* Size: 76 bytes (19 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: remap_stack_slots --map '24:40,26:42,28:44,30:46' */
/* @PostProc: swap_instructions 0x10 0x14 */
/* (2026-08-29: a dead `@PostProc: strip_instructions` was removed here. */
#include "types.h"

extern void ClearImage(RECT *r, s32 a1, s32 a2, s32 a3);
extern void DrawSync(s32 arg0);

void screen_clear(void) {
    RECT rect;

    rect.w = 640;
    rect.h = 480;
    rect.x = 0;
    rect.y = 0;
    ClearImage(&rect, 0, 0, 0);
    DrawSync(0);
}
