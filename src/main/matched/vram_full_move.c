/* Address: 0x800579B8 */
/* Size: 192 bytes (recorded) / 208 bytes over the absorbed head */
/* Region: Game (0x80041000-0x80086000) */
/* Compiler: IDO 5.2 */
/* @SymbolStart: 0x800579A8 */
/* @SymbolSize: 208 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:22 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
    s16 w;
    s16 h;
} Rect;

extern s32 activeBuff;
extern u8  bg_change_tbl1;
extern s32 date_effect_init_tbl0[];
extern s32 goto_step_bg_out_tbl0;

extern void MoveImage(Rect *r, s32 a, s32 b);
extern void indexed_byte_lsb_set(s32 a, s32 b);
extern void DrawSync(s32 a);

void vram_full_move(s32 arg0)
{
    s32 v;
    Rect r;

    r.x = 0;
    r.y = (s16)(activeBuff * 240);
    r.w = 320;
    r.h = (s16)arg0;
    v = 1 - bg_change_tbl1;
    date_effect_init_tbl0[v] = 0;
    switch (v) {
    case 0:
        MoveImage(&r, 0x2C0, 0);
        break;
    case 1:
        MoveImage(&r, 0x140, 0x100);
        break;
    }
    indexed_byte_lsb_set(1, 1 - bg_change_tbl1);
    goto_step_bg_out_tbl0 = 0;
    DrawSync(0);
}
