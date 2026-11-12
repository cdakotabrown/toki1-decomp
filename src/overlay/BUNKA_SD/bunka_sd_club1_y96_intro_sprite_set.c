/* bunka_sd_club1_y96_intro_sprite_set - BUNKA_SD overlay, Address: 0x8013838C, Size: 64 bytes (16 insns) */
#include "types.h"

extern void sprite_rec_set(s32, s32, s32);
extern void sub_sub_step_up(void);

void bunka_sd_club1_y96_intro_sprite_set(void)
{
    sprite_rec_set(0, 91, 1);
    sprite_rec_set(31, 87, 1);
    sub_sub_step_up();
}
