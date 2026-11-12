/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=occ:0 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x38 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x3c */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x4c */
/* @PostProc: swap_branch_operands --offsets 0x98 */
/* Address: 0x8013597C, Instructions: 146 -- BYTE-PERFECT (146/146, */
#include "types.h"
#include "tokimeki/class_w.h"

extern void menu_check();
extern void menu_cursor_move();
extern void option_menu_box_show();
extern void k_disp_switch();
extern void _sprite_set_box_shade();
extern void sub_sub_step_up();
extern void device_cmd_send();
extern void k_sub_reset(void);

extern volatile s8 menu_csr;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern u8  cw96_active[];
extern u8  menu_mode;
extern u32 pad_trig;
extern u8  step_mode;
extern CLASS_W class_w[];

void option_menu_mode_main(void)
{
    s32 i;
    s32 v;

    class_w[106].disp_flag = 128;
    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_cursor_move(1);
    option_menu_box_show();

    for (i = 0; i < 4; i++) {
        if (i == menu_csr) {
            k_disp_switch(i * 3 + 8, 1);
            k_disp_switch(i * 3 + 9, 1);
            k_disp_switch(i * 3 + 10, 1);
            v = i * 2 + 122;
            *(s16 *)(cw96_active + i * 68 + 772) = (s16)v;
        } else {
            k_disp_switch(i * 3 + 8, 0);
            k_disp_switch(i * 3 + 9, 0);
            k_disp_switch(i * 3 + 10, 0);
            v = i * 2 + 123;
            *(s16 *)(cw96_active + i * 68 + 772) = (s16)v;
        }
    }

    _sprite_set_box_shade(-133, 58, 256, 52, 8, 0x80808, 128);

    if ((pad_trig & 0x20) != 0) {
        switch (menu_csr) {
        case 0:
            menu_mode = 4;
            sub_sub_step_up();
            device_cmd_send(1281);
            break;
        case 1:
            menu_mode = 5;
            sub_sub_step_up();
            device_cmd_send(1281);
            break;
        case 2:
            menu_mode = 7;
            sub_sub_step_up();
            device_cmd_send(1281);
            break;
        case 3:
            menu_mode = 6;
            sub_sub_step_up();
            device_cmd_send(1281);
            break;
        }
    }

    if ((pad_trig & 0x40) != 0) {
        k_sub_reset();
        sub_sub_step_up();
        step_mode = 2;
    }
}
