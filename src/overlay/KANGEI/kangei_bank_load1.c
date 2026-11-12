/* kangei_bank_load1 - KANGEI overlay, Address: 0x80138D10, Size: 56 bytes (14 insns) */
#include "types.h"

extern void cd_read_start2(s32, void *, s32);
extern void kangei_bank_set1(void);
extern void sub_sub_step_up(void);

void kangei_bank_load1(void)
{
    cd_read_start2(3, (void *)0x801B0000, 0xAF43);
    kangei_bank_set1();
    sub_sub_step_up();
}
