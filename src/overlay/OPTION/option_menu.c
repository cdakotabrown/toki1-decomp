/* @SymbolStart: 0x80136FF8 */
/* Address: 0x80137004, Size: 440 bytes (110 instructions) */
/* @SymbolSize: 444 */
/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: patch_insn_reg_field 0x4c:rt:a3:a2 0x50:rs:a3:a2 0x54:rt:t0:t7 0x58:rd:t7:t8 0x5c:rd:t7:t8 0x5c:rs:t7:t8 0x60:rd:t7:t8 0x60:rt:t7:t8 0x64:rs:t0:t7 0x68:rt:a0:a1 0x6c:rs:a0:a1 0x6c:rt:a0:a1 0x70:rd:a1:a0 0x70:rs:t7:t8 0x78:rt:a1:a0 0x7c:rt:t9:t0 0x80:rt:t8:t9 0x88:rt:t8:t9 0x8c:rt:t9:t0 0x94:rt:a0:a1 0xd4:rt:t7:t8 0x170:rs:t7:t8 0x170:rt:t7:t8 0x178:rs:t7:t8 0x190:rt:t8:t7 0x194:rs:t8:t7 0x194:rt:t8:t7 0x19c:rs:t8:t7 0x1b0:rt:t1:t0 0x1b8:rt:t1:t0 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void menu_check();
extern void menu_cursor_move();
extern void option_menu_box_show();
extern void goto_sub_sub_step();
extern void device_cmd_send();
extern void sub_sub_step_up();
extern s32 get_option_count();

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s8 menu_csr;
extern u8 cw96_active[];
extern u32 pad_trig;
extern u8 bunkaken_flags;
extern u32 taco_rec;
extern u8 opt_flag;
extern u8 step_mode;
extern CLASS_W class_w[];

void option_menu(void)
{
    s32 a2;
    s32 v1;
    u8 *v0;
    u8 *a0;
    u8 *end;

    class_w[106].disp_flag = 128;
    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_cursor_move(1);
    option_menu_box_show();

    a2 = menu_csr;

    v0 = &cw96_active[0];
    end = &cw96_active[272];
    a0 = v0 + a2 * 68;
    v1 = 0;
    do {
        if (v0 == a0) {
            *(s16 *)(v0 + 772) = (s16)(v1 + 51);
        } else {
            *(s16 *)(v0 + 772) = (s16)(v1 + 52);
        }
        v0 += 68;
        v1 += 2;
    } while (v0 != end);

    if (pad_trig & 0x0020) {
        switch (a2) {
        case 0:
            if ((bunkaken_flags & 1) || (taco_rec & 1)) {
                goto_sub_sub_step(10);
                device_cmd_send(1281);
            }
            break;
        case 1:
            if (get_option_count() != 0) {
                goto_sub_sub_step(20);
                device_cmd_send(1281);
            }
            break;
        case 2:
            if (get_option_count() != 0) {
                goto_sub_sub_step(30);
                device_cmd_send(1281);
            }
            break;
        case 3:
            if (opt_flag != 0) {
                goto_sub_sub_step(40);
                device_cmd_send(1281);
            }
            break;
        }
    }

    if (pad_trig & 0x0040) {
        sub_sub_step_up();
        step_mode = 2;
    }
}
