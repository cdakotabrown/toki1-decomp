/* Address: 0x8005C3CC (true start 0x8005C3C4: the step byte load is */
/* Size: 712 bytes over the absorbed head (178 instructions) */
/* Region: Game (IDO 5.2) */
/* @SymbolStart: 0x8005C3C4 */
/* @SymbolSize: 712 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern void k_sub_reset();
extern void set_kanji_string();
extern void k_disp_start();
extern void k_sub_disp_start();
extern void goto_sub_sub_step();
extern void goto_sub_step();
extern void cursor_work_copy();

extern u8 step_mode;
extern s32 pad_trig;
extern u8 save_format_main_str0[];
extern u8 save_format_main_str1[];
extern u8 save_format_main_str2[];
extern u8 save_format_main_str3[];
extern u8 save_format_main_str4[];
extern u8 save_format_main_str5[];
extern u8 save_format_main_str6[];
extern u8 save_format_main_str7[];

save_format_main()
{
    switch (step_mode) {
    case 0:
        k_sub_reset();
        set_kanji_string(-128, 50, 15, save_format_main_str0, 0);
        k_disp_start(1);
        step_mode++;
        break;
    case 1:
        if (pad_trig & 0x860) {
            step_mode++;
        }
        break;
    case 2:
        k_sub_reset();
        set_kanji_string(-128, 50, 15, save_format_main_str1, 0);
        set_kanji_string(-128, 66, 0, save_format_main_str6, 0);
        set_kanji_string(-44, 66, 1, get_pad_str(), 0);
        set_kanji_string(-128, 82, 0, save_format_main_str7, 0);
        set_kanji_string(-44, 82, 2, get_pad_str2(), 0);
        k_sub_disp_start(5);
        step_mode++;
        break;
    case 3:
        if (pad_trig & 0x20) {
            if (card_format_run() == 1) {
                goto_sub_sub_step(0);
            } else {
                cursor_work_copy(1);
                goto_sub_step(0);
                goto_sub_sub_step(2);
            }
        } else if (pad_trig & 0x40) {
            step_mode++;
        }
        break;
    case 4:
        k_sub_reset();
        set_kanji_string(-128, 50, 15, save_format_main_str2, 0);
        set_kanji_string(-128, 66, 1, save_format_main_str3, 0);
        set_kanji_string(-128, 66, 15, save_format_main_str4, 0);
        set_kanji_string(-128, 82, 15, save_format_main_str5, 0);
        k_sub_disp_start(4);
        step_mode++;
        break;
    case 5:
        if (pad_trig & 0x20) {
            cursor_work_copy(1);
            goto_sub_step(0);
            goto_sub_sub_step(2);
        }
        break;
    }
}
