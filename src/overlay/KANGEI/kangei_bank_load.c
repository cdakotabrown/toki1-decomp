/* kangei_bank_load - KANGEI overlay, Address: 0x8013706C, Size: 60 bytes (15 insns) */
#include "types.h"

extern void cd_read_start2(s32, void *, s32);
extern void kangei_work_desc_set(void);
extern void sub_sub_step_up(void);

void kangei_bank_load(void)
{
    cd_read_start2(110, (void *)0x801B4400, 0xBBB3);
    kangei_work_desc_set();
    sub_sub_step_up();
}
