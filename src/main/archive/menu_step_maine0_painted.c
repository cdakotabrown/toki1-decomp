/* Function: func_8005BD70 (func_8005BD70) */
/* Address: 0x8005BD70 */
/* Size: 156 bytes (39 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: custom_script custom_func_8005BD70.py */
#include "types.h"

extern void device_cmd_send(s32);
extern void k_sub_reset(void);
extern void set_kanji_string(s32, s32, s32, void*, s32);
extern void k_sub_disp_start(s32);
extern void goto_sub_sub_step(s32);

extern u8 step_mode;
extern s32 pad_trig;
extern s32 menu_step_maine0_str0;

s32 func_8005BD70(s32 mode) {
    s32 ret = mode;
    switch (mode) {
    case 0:
        device_cmd_send(1282);
        k_sub_reset();
        set_kanji_string(-128, 50, 1, &menu_step_maine0_str0, 0);
        k_sub_disp_start(1);
        step_mode++;
        break;
    case 1:
        if (pad_trig & 0x860) {
            goto_sub_sub_step(6);
        }
        break;
    }
    return ret;
}
