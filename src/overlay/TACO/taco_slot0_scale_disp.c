/* @CompileFlags: -cckr */
#include "types.h"

extern s32 taco_menu_anim_cnt;
extern s32 taco_menu_val;
extern u8 taco_rensya_flag;
extern u8 taco_slot0_scale_disp_str0[];
extern u8 taco_slot0_scale_disp_str1[];
extern u8 taco_slot0_scale_disp_str2[];
extern u8 taco_slot0_scale_disp_str3[];

extern void taco_glyph_str_disp();

void taco_slot0_scale_disp()
{
    s32 q;

    if (taco_menu_anim_cnt < 15) {
        q = ((15 - taco_menu_anim_cnt) << 12) / 15;
        switch (taco_menu_val % 2) {
        case 0:
            taco_glyph_str_disp(taco_slot0_scale_disp_str0, q, 4096, 5, 3, 42, -16);
            break;
        case 1:
            taco_glyph_str_disp(taco_slot0_scale_disp_str1, q, 4096, 5, 3, 42, -16);
            break;
        }
    } else {
        q = ((taco_menu_anim_cnt << 12) - 61440) / 15;
        switch (taco_rensya_flag % 2) {
        case 0:
            taco_glyph_str_disp(taco_slot0_scale_disp_str2, q, 4096, 5, 3, 42, -16);
            break;
        case 1:
            taco_glyph_str_disp(taco_slot0_scale_disp_str3, q, 4096, 5, 3, 42, -16);
            break;
        }
    }
}
