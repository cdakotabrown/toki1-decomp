/* Address: 0x801330FC */
/* Size: 212 bytes (53 insns) */
/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic1 */
/* @PostProc: patch_stack_complete --old 56 --new 64 */
/* @PostProc: swap_instructions --offset1 0xac --offset2 0xb4 */
#include "types.h"

extern s32 srn_vram_set();
extern s32 srn_init();
extern s32 mod_init();
extern s32 MoveImage();
extern s32 taiiku_com_buf;
extern s32 taiiku_com_buf_1;
extern s32 taiiku_com_buf_2;

extern s32 bukatu_tbl;
extern s32 bukatu_tbl_end[1];
extern s32 tpage_buf[];

typedef struct {
    s16 x, y, w, h;
} TAIIKU_RECT;

void taiiku_module_init2(void) {
    TAIIKU_RECT rect;
    s32 *v1;

    rect.x = 320;
    rect.y = 0;
    rect.w = 32;
    rect.h = 112;
    mod_init(0, taiiku_com_buf_1, taiiku_com_buf);
    srn_init(0, taiiku_com_buf_2, 0);
    srn_vram_set(0, 5, 1408, 240);
    MoveImage(&rect, 960, 0);
    tpage_buf[5] = 8;
    tpage_buf[6] = 8;
    tpage_buf[7] = 8;
    v1 = &bukatu_tbl;
    do {
        v1 += 4;
        *(s32*)((char*)v1 + 4656) = 8;
        *(s32*)((char*)v1 + 4660) = 8;
        *(s32*)((char*)v1 + 4664) = 8;
        *(s32*)((char*)v1 + 4652) = 8;
    } while (v1 != bukatu_tbl_end);
}
