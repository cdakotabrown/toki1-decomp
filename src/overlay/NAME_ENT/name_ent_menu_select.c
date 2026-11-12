/* @SymbolSize: 316 */
/* @PostProc: patch_insn_reg_field 0x10c:rt:v0:t8 0x110:rt:v0:t8 0x110:rs:v0:t8 0x114:rt:t8:t9 0x118:rs:v0:t8 0x120:rt:t8:t9 */
/* name_ent_menu_select - NAME_ENT overlay, Address: 0x80142864, Size: 316 bytes (79 insns). */
#include "types.h"

extern void menu_check();
extern void menu_cursor_move();
extern void name_ent_sprite_off();
extern void goto_sub_sub_step();
extern void printf();
extern void sprite_init();
extern s32 name_ent_button_check();

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 pad_trig;
extern s8 menu_csr;
extern u8 name_ent_menu_select_str0[];
extern u8 name_ent_menu_select_str1[];
extern u8 name_ent_menu_select_str2[];
extern u8 name_ent_menu_select_str3[];
extern s32 name_ent_view_page;

void name_ent_menu_select(void)
{
    s32 buttons;
    s8 dir;
    s32 result;

    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_cursor_move(1);

    buttons = pad_trig;
    if (buttons & 0x0020) {
        dir = menu_csr;
        switch (dir) {
        case 0:
            name_ent_sprite_off();
            goto_sub_sub_step(192);
            printf(name_ent_menu_select_str0);
            break;
        case 1:
            name_ent_sprite_off();
            goto_sub_sub_step(160);
            printf(name_ent_menu_select_str1);
            break;
        case 2:
            name_ent_sprite_off();
            goto_sub_sub_step(224);
            printf(name_ent_menu_select_str2);
            break;
        case 3:
            goto_sub_sub_step(0);
            printf(name_ent_menu_select_str3);
            break;
        }
        sprite_init(0, 64);

        dir = menu_csr;
        if (dir >= 0) {
            name_ent_view_page = 1;
        }
    }
    result = name_ent_button_check();
    if (result != 0) {
        sprite_init(0, 64);
    }
}
