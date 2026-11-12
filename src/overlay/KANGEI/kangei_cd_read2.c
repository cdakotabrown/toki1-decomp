/* kangei_cd_read2 - KANGEI overlay, Address: 0x80133CFC, Size: 60 bytes (15 insns) */
#include "types.h"

extern void cd_read_start2(s32, void *, s32);
extern void kangei_girl0_desc_set(void);
extern void sub_sub_step_up(void);

void
kangei_cd_read2(void)
{
    cd_read_start2(45, (void *)0x801B4400, 0xAE9B);
    kangei_girl0_desc_set();
    sub_sub_step_up();
}
