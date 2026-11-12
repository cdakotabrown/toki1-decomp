/* Function: fade_up */
/* Address: 0x8007E5B4 */
/* Size: 192 bytes (48 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 24 --new 40 */
/* @PostProc: custom_script custom_func_8007E5B4.py */
#include "types.h"

extern void sub_sub_step_up(void);
extern volatile u32 Sprites;
extern volatile u32 sprite1_attribute;
extern volatile u32 fade_flag;

s32 fade_up(u32 a0) {
    u32 cb0;
    u32 mask;
    u8 narrowed;

    if (a0 < 128) {
        a0 += 8;
        narrowed = (u8)(a0 & 0xFF);
        a0 = narrowed;
    } else {
        sub_sub_step_up();

        cb0 = Sprites;
        mask = 0x7FFFFFFF;
        fade_flag = 1;
        Sprites = cb0 & mask;
        sprite1_attribute = sprite1_attribute & mask;

        return 0;
    }

    narrowed = (u8)(a0 & 0xFF);
    *(volatile u8*)0x80129CE9 = narrowed;
    *(volatile u8*)0x80129CE8 = narrowed;
    *(volatile u8*)0x80129CC6 = narrowed;
    *(volatile u8*)0x80129CC5 = narrowed;
    *(volatile u8*)0x80129CC4 = narrowed;
    *(volatile u8*)0x801279F3 = a0;
    *(volatile u8*)0x800BBE44 = a0;
    *(volatile u8*)0x80129CEA = a0;

    return a0;
}
