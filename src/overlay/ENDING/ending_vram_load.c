/* @CompileFlags: -Wc,-pic0 */
/* @SymbolSize: 144 */
/* @PostProc: remap_stack_slots --from 48 --to 44 --from 50 --to 46 --from 52 --to 48 --from 54 --to 50 */
/* ending_vram_load - ENDING overlay, Address: 0x801336B8, Size: 168 bytes (42 instructions) */
#include "types.h"

extern void LoadImage();
extern void sub_sub_step_up();

extern s32 etc_rec_tbl0;
extern s32 etc_rec_tbl1;
extern s32 etc_rec_tbl2;
extern s32 etc_rec_tbl3;
extern s32 etc_rec_tbl4;
extern s32 etc_rec_tbl5;

void ending_vram_load(void)
{
    RECT r;
    s32 one0, one1, one2, one3, one4, one5;

    r.x = 320;
    r.y = 128;
    r.w = 384;
    r.h = 128;
    LoadImage(&r, (s32)0x80180000);

    one0 = 1;
    etc_rec_tbl0 = one0;
    one1 = 1;
    etc_rec_tbl1 = one1;
    one2 = 1;
    etc_rec_tbl2 = one2;
    one3 = 1;
    etc_rec_tbl3 = one3;
    one4 = 1;
    etc_rec_tbl4 = one4;
    one5 = 1;
    etc_rec_tbl5 = one5;
    sub_sub_step_up();
}
