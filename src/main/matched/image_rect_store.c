/* Address: 0x800445FC */
/* Size: 68 bytes (17 instructions) */
/* Region: Game (0x80041000-0x80085FFF) - 48-byte frame */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_registers t0 t8 */
/* @PostProc: swap_registers t1 t9 */
#include "types.h"

typedef struct {
    s16 x, y, w, h;
} RECT8;

extern void StoreImage(RECT8 *rect, s32 arg1);

void image_rect_store(s32 x, s32 y, s16 w, s16 h, s32 arg4) {
    RECT8 rect;
    s32 pad[3];
    rect.x = *(u16*)&x;
    rect.y = *(u16*)&y;
    rect.w = w;
    rect.h = h;
    StoreImage(&rect, arg4);
}
