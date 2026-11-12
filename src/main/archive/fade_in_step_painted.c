/* Address: 0x8007E97C */
/* Size: 76 bytes (19 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 24 --new 40 */
/* @PostProc: custom_script custom_func_8007E97C.py */
#include "types.h"

extern u8 fade_bri;
extern void sub_sub_step_up(void);

void func_8007E97C(u32 value) {
    u32 result;

    result = value;
    if (value >= 128U) {
        result = 128;
    }
    fade_bri = result;

    if (result >= 125U) {
        fade_bri = 128;
        sub_sub_step_up();
    }
}
