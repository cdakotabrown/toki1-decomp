/* @CompileFlags: -Wc,-pic1 */
/* @SymbolStart: 0x80145A8C */
/* save_file_shade_disp - ETC overlay, Address: 0x80145A94 */
/* @SymbolSize: 264 */
/* @PostProc: patch_stack_complete --old 64 --new 72 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x88 */
/* @PostProc: scheduler_normalizer */
#include "types.h"

extern s32 _sprite_set_box_shade(s32, s32, s32, s32, s32, s32, s32);
extern s32 dtd_on(s32);
extern s32 dtd_on_tpage(s32, s32, s32, s32, s32);

extern u8 screen_bri;
extern u8 now_sub_sub_step;

extern u8 now_sub_step;

s32 save_file_shade_disp(void)
{
    s32 v;
    s32 n, half, col1;
    s32 m, col2;

    if (now_sub_step != 0) goto middle;
    if (now_sub_sub_step >= 2U) goto middle;
    return 0;

middle:
    v = screen_bri;
    n = v * 3;
    half = n / 2;
    col1 = half * 0x10101;
    _sprite_set_box_shade(-160, -87, 283, 125, 2, col1, 2);
    dtd_on(2);
    m = v / 3;
    col2 = m * 0x40401;
    _sprite_set_box_shade(-133, 46, 266, 52, 2, col2, 129);
    return dtd_on_tpage(0, 0, 2, 0, 0);
}
