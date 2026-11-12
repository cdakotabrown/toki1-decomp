/* Function: func_8007EAE8 */
/* Address: 0x8007EAE8 */
/* Size: 264 bytes (66 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: patch_stack_complete --old 24 --new 40 */
/* @PostProc: strip_leading_instructions --count 6 */
/* @PostProc: custom_script custom_func_8007EAE8.py */
#include "types.h"

extern u8 bustup2_cw_disp;
extern u8 bustup_cw_disp;
extern u8 bustup2_cw_bright;
extern u8 fade_bri;
extern s32 Sprites;
extern s8 sprite0_r;
extern s8 sprite0_g;
extern u8 sprite0_b;
extern s32 sprite1_attribute;
extern s8 sprite1_r;
extern s8 sprite1_g;
extern u8 sprite1_b;
extern s32 bustup_on;
extern s32 fade_flag;

extern void fade_out_step(void);

void func_8007EAE8(void) {
    u32 cond;
    u8 val;

    cond = fade_bri;
    if (cond < 8U) {
        bustup_on = 0;
        bustup2_cw_disp = bustup2_cw_disp & 0xFF7F;
        fade_flag = 0;
        bustup_cw_disp = bustup_cw_disp & 0xFF7F;
        sprite0_g = 128;
        sprite0_r = 128;
        sprite1_g = 128;
        sprite1_r = 128;
        Sprites = Sprites | 0x80000000;
        sprite1_attribute = sprite1_attribute | 0x80000000;
        sprite0_b = 128;
        sprite1_b = 128;
    }
    fade_out_step();
    val = fade_bri;
    bustup2_cw_bright = val;
    sprite0_g = val;
    sprite0_r = val;
    sprite1_g = val & 0xFF;
    sprite1_r = val & 0xFF;
    sprite0_b = val;
    sprite1_b = val;
}
