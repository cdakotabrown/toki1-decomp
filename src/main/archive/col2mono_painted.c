/* Function: col2mono */
/* Address: 0x800579B8 */
/* Size: 192 bytes (48 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 8 */
/* @PostProc: patch_stack_complete --old 40 --new 64 */
/* @PostProc: custom_script custom_col2mono.py */
#include "types.h"

extern s32 activeBuff;
extern u8 bg_change_tbl1;
extern s32 date_effect_init_tbl0[];
extern s32 goto_step_bg_out_tbl0;

extern void MoveImage(RECT *, s32, s32);
extern void func_80057780(s32, s32);
extern void DrawSync(s32);

void col2mono(s16 arg0) {
    s32 v0;
    RECT rect;

    rect.x = 0;
    rect.y = activeBuff * 240;
    rect.w = 320;
    rect.h = arg0;
    v0 = 1 - bg_change_tbl1;
    date_effect_init_tbl0[v0] = 0;

    if (v0 == 0) goto case0;
    if (v0 == 1) goto case1;
    v0 = 1;
    goto merge;

case0:
    MoveImage(&rect, 704, 0);
    v0 = 1 - bg_change_tbl1;
    goto merge;

case1:
    MoveImage(&rect, 320, 256);
    v0 = 1 - bg_change_tbl1;

merge:
    func_80057780(1, v0);
    goto_step_bg_out_tbl0 = 0;
    DrawSync(0);
}
