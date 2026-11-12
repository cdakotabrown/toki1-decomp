/* kangei_sprite_desc_set -- KANGEI overlay, Address: 0x80135F10, 29 insns. */
#include "types.h"

extern s32 kangei_sprite_buf_tbl[];

void kangei_sprite_desc_set(void)
{
    kangei_sprite_buf_tbl[0] = 0x801B4400;
    kangei_sprite_buf_tbl[1] = 0x801B6400;
    kangei_sprite_buf_tbl[2] = 0x801BA400;
    kangei_sprite_buf_tbl[3] = 0x801BE400;
    kangei_sprite_buf_tbl[4] = 0x801C2400;
    kangei_sprite_buf_tbl[5] = 0x801C6400;
    kangei_sprite_buf_tbl[6] = 0x801CA400;
}
