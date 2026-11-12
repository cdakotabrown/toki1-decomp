/* Function: touzyou_girl1 */
/* Address: 0x8007ECA8 */
/* Size: 260 bytes (65 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_touzyou_girl1.py */
/* @PostProc: patch_stack_complete --old 32 --new 40 */
#include "types.h"

extern void bg_change(s32);
extern void func_80057760(s32);
extern void sub_sub_step_up(void);

extern volatile u32 Sprites;
extern volatile u32 sprite1_attribute;
extern volatile u8 sprite0_g;
extern volatile u8 sprite0_r;
extern volatile u8 sprite1_g;
extern volatile u8 sprite1_r;
extern volatile u8 sprite1_b;
extern volatile u8 sprite0_b;
extern volatile s32 fade_flag;
extern volatile u8 bg_change_tbl2;

s32 sprite_fade(u32 a0) {
    u8 val;

    if (a0 < 8U) goto if_body;
    val = (u8)(a0 - 8);
    goto else_body;
if_body:
    bg_change(1);
    func_80057760(1);
    sub_sub_step_up();
    Sprites = Sprites | 0x80000000;
    sprite1_attribute = sprite1_attribute | 0x80000000;
    sprite0_g = 128;
    sprite0_r = 128;
    sprite1_g = 128;
    sprite1_r = 128;
    fade_flag = 1;
    sprite1_b = 128;
    sprite0_b = 128;
    return 0;
else_body:
    sprite1_g = val;
    sprite1_r = val;
    sprite0_g = val;
    sprite0_r = val;
    bg_change_tbl2 = val;
    sprite1_b = val;
    sprite0_b = val;
    bg_change(1);
    return val;
}
