/* bunka_sd_sprite_set6 - Address: 0x801371BC, Size: 112 bytes (28 insns) */
#include "types.h"

extern void bunka_sd_class_flag_set(s32, s32, s32);
extern void sub_sub_step_up(void);

void bunka_sd_sprite_set6(void)
{
    bunka_sd_class_flag_set(0, 68, 1);
    bunka_sd_class_flag_set(1, 24, 1);
    bunka_sd_class_flag_set(2, 0, 1);
    bunka_sd_class_flag_set(3, 1, 1);
    bunka_sd_class_flag_set(31, 65, 1);
    sub_sub_step_up();
}
