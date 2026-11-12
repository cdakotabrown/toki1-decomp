/* Address: 0x801331D0 */
/* Size: 136 bytes (34 insns) */
/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=1 */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x70 */
#include "types.h"

extern s32 LoadImage();
extern s32 taiiku_com_buf_3;
extern s32 screen_w;

typedef struct {
    s16 x, y, w, h;
} TAIIKU_RECT;

void taiiku_img_load(void) {
    s32 x;
    s32 *v0;
    TAIIKU_RECT rect;
    s32 *bound;

    for (x = 320; x < 512; x += 64) {
        rect.x = x;
        rect.y = 112;
        rect.w = 64;
        rect.h = 128;
        LoadImage(&rect, taiiku_com_buf_3);
    }

    bound = &screen_w + 3;
    v0 = &screen_w;
loop2:
    v0++;
    *(s32*)((char*)v0 + 4792) = 8;
    if (v0 != bound) goto loop2;
}
