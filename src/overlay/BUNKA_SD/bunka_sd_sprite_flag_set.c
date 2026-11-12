/* bunka_sd_sprite_flag_set - BUNKA_SD overlay, Address: 0x80133CC4, Size: 144 bytes (36 insns) */
#include "types.h"

extern void sprite_flag_set(s32, s32, s32);
extern void sub_sub_step_up(void);

void bunka_sd_sprite_flag_set(void)
{
    sprite_flag_set(31, 191, 0);
    sprite_flag_set(30, 192, 1);
    sprite_flag_set(29, 193, 1);
    sprite_flag_set(8, 7, 1);
    sprite_flag_set(2, 77, 1);
    sprite_flag_set(1, 46, 1);
    sprite_flag_set(0, 80, 1);
    sub_sub_step_up();
}
