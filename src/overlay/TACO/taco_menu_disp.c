/* @CompileFlags: -cckr -Wc,-pic0 */
/* @SymbolSize: 1384 */
/* Address: 0x80135A14 */
/* Size: 1384 bytes (346 instructions; the .sym entry runs 16 bytes into */
#include "types.h"

extern u8 taco_menu_disp_str0[];
extern u8 taco_menu_disp_str1[];
extern u8 taco_menu_disp_str2[];
extern u8 taco_menu_disp_str3[];
extern u8 taco_menu_disp_str4[];
extern u8 taco_menu_disp_str5[];
extern u8 taco_menu_disp_str6[];
extern u8 taco_menu_disp_str7[];
extern u8 taco_menu_disp_str8[];
extern u8 taco_menu_disp_str9[];
extern u8 taco_menu_disp_str10[];
extern u8 taco_menu_disp_str11[];
extern u8 taco_menu_disp_str12[];
extern u8 taco_menu_disp_str13[];
extern u8 taco_menu_disp_str14[];
extern u8 taco_menu_disp_str15[];
extern u8 taco_menu_disp_str16[];

extern u8 taco_rensya_flag;
extern u8 taco_turn_btn;
extern u8 taco_shot_btn;
extern u8 taco_burst_btn;
extern s32 taco_submenu_sel;
extern s32 taco_menu_edit_flag;

taco_menu_disp()
{
    s32 col[7];
    u32 sel;
    s32 i;
    s32 k;
    s32 m;

    for (i = 0; i < 6; i++) { col[i] = 3; }
    col[taco_submenu_sel] = 5;

    _sprite_set_box_shade(-136, -28, 270, 120, 4, 0x3F3F3F, 129);

    taco_glyph_str_disp(taco_menu_disp_str0, 4096, 4096, col[5], 3, -48, -64);
    taco_glyph_str_disp(taco_menu_disp_str1, 4096, 4096, col[0], 3, -120, -16);
    taco_glyph_str_disp(taco_menu_disp_str2, 4096, 4096, col[1], 3, -120, 8);
    taco_glyph_str_disp(taco_menu_disp_str3, 4096, 4096, col[2], 3, -120, 32);
    taco_glyph_str_disp(taco_menu_disp_str4, 4096, 4096, col[3], 3, -120, 56);
    taco_glyph_str_disp(taco_menu_disp_str5, 4096, 4096, col[4], 3, -120, 80);

    if (taco_menu_edit_flag != 1 || taco_submenu_sel != 0) {
        sel = taco_rensya_flag;
        sel = sel & 1;
        switch (sel) {
        case 0:
            taco_glyph_str_disp(taco_menu_disp_str6, 4096, 4096, col[0], 3, 42, -16);
            break;
        case 1:
            taco_glyph_str_disp(taco_menu_disp_str7, 4096, 4096, col[0], 3, 42, -16);
            break;
        }
    }

    if (taco_menu_edit_flag != 1 || taco_submenu_sel != 1) {
        sel = taco_turn_btn;
        sel = sel % 3;
        switch (sel) {
        case 0:
            taco_glyph_str_disp(taco_menu_disp_str8, 4096, 4096, col[1], 3, 42, 8);
            break;
        case 1:
            taco_glyph_str_disp(taco_menu_disp_str9, 4096, 4096, col[1], 3, 42, 8);
            break;
        case 2:
            taco_glyph_str_disp(taco_menu_disp_str10, 4096, 4096, col[1], 3, 42, 8);
            break;
        }
    }

    if (taco_menu_edit_flag != 1 || taco_submenu_sel != 2) {
        sel = taco_shot_btn;
        sel = sel % 3;
        switch (sel) {
        case 0:
            taco_glyph_str_disp(taco_menu_disp_str11, 4096, 4096, col[2], 3, 42, 32);
            break;
        case 1:
            taco_glyph_str_disp(taco_menu_disp_str12, 4096, 4096, col[2], 3, 42, 32);
            break;
        case 2:
            taco_glyph_str_disp(taco_menu_disp_str13, 4096, 4096, col[2], 3, 42, 32);
            break;
        }
    }

    if (taco_menu_edit_flag != 1 || taco_submenu_sel != 3) {
        sel = taco_burst_btn;
        sel = sel % 3;
        switch (sel) {
        case 0:
            taco_glyph_str_disp(taco_menu_disp_str14, 4096, 4096, col[3], 3, 42, 56);
            break;
        case 1:
            taco_glyph_str_disp(taco_menu_disp_str15, 4096, 4096, col[3], 3, 42, 56);
            break;
        case 2:
            taco_glyph_str_disp(taco_menu_disp_str16, 4096, 4096, col[3], 3, 42, 56);
            break;
        }
    }
}
