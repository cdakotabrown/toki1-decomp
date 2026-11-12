/* Debug Symbol: cursor_init (from o.bin, high match) */
/* Address: 0x8006BCC8 */
/* Size: 324 bytes (81 instructions) */
/* Region: Game */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: remap_stack_slots --map 30:46,28:44 */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xbc */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s8 cursor_cw_prio;
extern s8 cursor_cw_mode;
extern u8 cursor_cw_disp;
extern s8 cursor_cw_flag4;
extern s8 cursor_cw_sel;
extern s32 cursor_cw_img;
extern s32 cursor_cw_anim_tbl;
extern s16 cursor_cw_aux14;
extern s16 cursor_cw_anim;
extern s16 cursor_cw_frame;
extern s32 cursor_cw_hdr;
extern s32 cursor_cw_attr;
extern s16 basyo_init_tbl3;
extern u8 basyo_init_tbl1[];
extern u8 basyo_init_tbl2[];
extern u8 _sys_default_tpage_set_tbl2[];
extern u8 cursor_init_tbl0;

extern void class_clear_n(s32 arg0);

void cursor_init(s32 arg0) {
    s16 savedX;
    s16 savedY;
    s32 temp;

    savedX = cursor_cw_x;
    savedY = cursor_cw_y;

    class_clear_n(0);

    cursor_cw_prio = 1;
    cursor_cw_mode = 64;
    cursor_cw_attr = 0;
    cursor_cw_disp = 132;
    cursor_cw_img = (s32)basyo_init_tbl1;
    cursor_cw_anim_tbl = (s32)basyo_init_tbl2;
    cursor_cw_hdr = (s32)_sys_default_tpage_set_tbl2;
    cursor_cw_aux14 = basyo_init_tbl3;
    cursor_cw_flag4 = 8;
    cursor_cw_sel = 1;

    temp = cursor_init_tbl0;
    cursor_cw_anim = (s16)(temp * 0);

    switch (arg0) {
        case 0:
        default:
            if (temp != 0 && temp >= 155) {
                cursor_cw_frame = (s16)temp;
                if (temp < 174) {
                    break;
                }
            }
            cursor_cw_frame = 0;
            break;
        case 1:
            if (temp != 0 && temp >= 155) {
                cursor_cw_frame = (s16)temp;
                if (temp < 174) {
                    break;
                }
            }
            cursor_cw_frame = 0;
            break;
    }

    cursor_cw_x = savedX;
    cursor_cw_y = savedY;
}
