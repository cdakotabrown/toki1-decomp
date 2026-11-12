/* Address: 0x8005BE14 */
/* Size: 172 bytes (43 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_menu_check_1.py */
#include "types.h"

extern u8 step_mode;
extern u32 pad_trig;
extern u8 menu_step_mainff_str0[];

extern void device_cmd_send(s32);
extern void k_sub_reset(void);
extern void set_kanji_string(s32, s32, s32, u8*, s32);
extern void k_sub_disp_start(s32);
extern void cursor_work_copy(s32);
extern void goto_sub_step(s32);
extern void goto_sub_sub_step(s32);

s32 menu_step_mainff(s32 mode) {
    switch (mode) {
    case 0:
        device_cmd_send(1282);
        k_sub_reset();
        set_kanji_string(-128, 50, 1, menu_step_mainff_str0, 0);
        k_sub_disp_start(1);
        step_mode = step_mode + 1;
        break;
    case 1:
        if (pad_trig & 0x860) {
            cursor_work_copy(1);
            goto_sub_step(0);
            goto_sub_sub_step(2);
        }
        break;
    }
    return mode;
}
