/* bunka_sd_club1_y96_cue_sprite_set - BUNKA_SD overlay, Address: 0x801383CC, Size: 96 bytes (24 insns) */
#include "types.h"

extern void sprite_rec_set(s32, s32, s32);
extern void sub_sub_step_up(void);

void bunka_sd_club1_y96_cue_sprite_set(void)
{
    sprite_rec_set(0, 91, 1);
    sprite_rec_set(31, 88, 1);
    sprite_rec_set(5, 0, 1);
    sprite_rec_set(4, 8, 1);
    sub_sub_step_up();
}
