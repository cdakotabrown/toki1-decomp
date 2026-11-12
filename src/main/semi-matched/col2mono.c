/* Function: col2mono */
/* Address: 0x800579B8 */
/* Size: 192 bytes (48 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x800579A8 */
/* @SymbolSize: 208 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @PostProc: strip_leading_instructions --count auto */
/* @Measured: 2026-09-24 28/52 exact (53.8%), compiled 52 vs target 52 words, Fixup 0 -- SEMI */
#include "types.h"

extern s32 activeBuff;
extern u8 bg_change_tbl1;
extern s32 date_effect_init_tbl0[];
extern s32 goto_step_bg_out_tbl0;

extern void MoveImage(RECT *, s32, s32);
extern void indexed_byte_lsb_set(s32, s32);
extern void DrawSync(s32);

void col2mono(s16 arg0)
{
    s32 v0;
    RECT rect;

    rect.x = 0;
    rect.y = activeBuff * 240;
    rect.w = 320;
    rect.h = arg0;
    v0 = 1 - bg_change_tbl1;
    date_effect_init_tbl0[v0] = 0;

    switch (v0) {
    case 0:
        MoveImage(&rect, 704, 0);
        v0 = 1 - bg_change_tbl1;
        break;
    case 1:
        MoveImage(&rect, 320, 256);
        v0 = 1 - bg_change_tbl1;
        break;
    }

    indexed_byte_lsb_set(1, v0);
    goto_step_bg_out_tbl0 = 0;
    DrawSync(0);
}
