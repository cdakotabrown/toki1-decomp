/* kangei_bank_load2 - KANGEI overlay, Address: 0x80138CD8, Size: 56 bytes (14 insns) */
#include "types.h"

extern void cd_read_start2(s32, void *, s32);
extern void kangei_bank_set2(void);
extern void sub_sub_step_up(void);

void kangei_bank_load2(void)
{
    cd_read_start2(22, (void *)0x801B0000, 0xAF0D);
    kangei_bank_set2();
    sub_sub_step_up();
}
