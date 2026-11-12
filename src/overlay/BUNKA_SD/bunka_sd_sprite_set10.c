/* bunka_sd_sprite_set10 - BUNKA_SD overlay, Address: 0x8013722C, Size: 144 bytes (36 insns) */
#include "types.h"

extern void bunka_sd_class_flag_set(s32, s32, s32);
extern void sub_sub_step_up(void);

void bunka_sd_sprite_set10(void)
{

    bunka_sd_class_flag_set(1, 255, 0);

    bunka_sd_class_flag_set(2, 255, 0);

    bunka_sd_class_flag_set(3, 255, 0);

    bunka_sd_class_flag_set(4, 255, 0);

    bunka_sd_class_flag_set(5, 255, 0);

    bunka_sd_class_flag_set(0, 68, 1);

    bunka_sd_class_flag_set(31, 66, 1);

    sub_sub_step_up();
}
