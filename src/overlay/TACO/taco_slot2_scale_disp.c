/* @SymbolStart: 0x80136448 */
/* @SymbolSize: 592 */
#include "types.h"

extern s32 taco_menu_anim_cnt;
extern s32 taco_menu_val;
extern u8 taco_shot_btn;
extern u8 taco_slot2_scale_disp_str0[];
extern u8 taco_slot2_scale_disp_str1[];
extern u8 taco_slot2_scale_disp_str2[];
extern u8 taco_slot2_scale_disp_str3[];
extern u8 taco_slot2_scale_disp_str4[];
extern u8 taco_slot2_scale_disp_str5[];

extern void taco_glyph_str_disp(s32, s32, s32, s32, s32, s32, s32);

void taco_slot2_scale_disp(void)
{
    s32 g;
    s32 q;

    g = taco_menu_anim_cnt;
    if (g < 15) {
        q = ((15 - g) << 12) / 15;
        g = 0;
        switch (taco_menu_val % 3) {
        case 0:
            taco_glyph_str_disp((s32)taco_slot2_scale_disp_str0, q, 4096, 5, 3, 42, 32);
            break;
        case 1:
            taco_glyph_str_disp((s32)taco_slot2_scale_disp_str1, q, 4096, 5, 3, 42, 32);
            break;
        case 2:
            taco_glyph_str_disp((s32)taco_slot2_scale_disp_str2, q, 4096, 5, 3, 42, 32);
            break;
        }
    } else {
        q = ((g << 12) - 61440) / 15;
        g = 0;
        switch ((u32)taco_shot_btn % 3) {
        case 0:
            taco_glyph_str_disp((s32)taco_slot2_scale_disp_str3, q, 4096, 5, 3, 42, 32);
            break;
        case 1:
            taco_glyph_str_disp((s32)taco_slot2_scale_disp_str4, q, 4096, 5, 3, 42, 32);
            break;
        case 2:
            taco_glyph_str_disp((s32)taco_slot2_scale_disp_str5, q, 4096, 5, 3, 42, 32);
            break;
        }
    }
}
