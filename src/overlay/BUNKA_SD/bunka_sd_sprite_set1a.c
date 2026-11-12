/* bunka_sd_sprite_set1a - BUNKA_SD overlay, Address: 0x801372BC, Size: 96 bytes (24 insns) */
#include "types.h"

extern void bunka_sd_class_flag_set(s32, s32, s32);
extern void sub_sub_step_up(void);

void bunka_sd_sprite_set1a(void)
{
    bunka_sd_class_flag_set(0, 68, 1);
    bunka_sd_class_flag_set(1, 38, 1);
    bunka_sd_class_flag_set(2, 39, 1);
    bunka_sd_class_flag_set(31, 67, 1);
    sub_sub_step_up();
}
