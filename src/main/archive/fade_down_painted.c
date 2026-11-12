/* Function: fade_down */
/* Address: 0x8007E780 */
/* Size: 288 bytes (72 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: patch_stack_complete --old 24 --new 40 */
/* @PostProc: strip_leading_instructions --count 6 */
/* @PostProc: custom_script custom_func_8007E780.py */
#include "types.h"

extern u8 bg_change_tbl2;
extern u8 bustup2_cw_bright;
extern u8 bustup2_cw_disp;
extern u8 bustup_cw_disp;
extern s32 Sprites;
extern s32 sprite1_attribute;
extern u8 sprite0_r;
extern u8 sprite0_g;
extern u8 sprite0_b;
extern u8 sprite1_r;
extern u8 sprite1_g;
extern u8 sprite1_b;
extern s32 bustup_on;
extern s32 fade_flag;

extern void sub_sub_step_up(void);

s32 fade_down(void) {
    s32 val;
    s32 masked;

    val = bg_change_tbl2;

    if ((u32)val >= 8) {
        val = (u8)(val - 8);
        masked = val & 0xFF;
        sprite1_g = masked;
        sprite1_r = masked;
        sprite0_g = masked;
        sprite0_r = masked;
        bustup2_cw_bright = val;
        bg_change_tbl2 = val;
        sprite1_b = val;
        sprite0_b = masked;
        return masked;
    }

    bustup2_cw_disp = bustup2_cw_disp & 0x7F;
    bustup_cw_disp = bustup_cw_disp & 0x7F;
    sub_sub_step_up();
    Sprites = Sprites | (s32)0x80000000;
    sprite1_attribute = sprite1_attribute | (s32)0x80000000;
    sprite0_g = 128;
    sprite0_r = 128;
    sprite1_g = 128;
    sprite1_r = 128;
    bustup_on = 0;
    fade_flag = 1;
    sprite1_b = 128;
    sprite0_b = 128;
    return 0;
}
