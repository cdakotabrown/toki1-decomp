/* kangei_data_load - KANGEI overlay, Address: 0x801324D8, Size: 60 bytes (15 insns) */
#include "types.h"

extern void cd_read_start2(s32, void *, s32);
extern void kangei_ptr_tbl_init(void);
extern void sub_sub_step_up(void);

void
kangei_data_load(void)
{
    cd_read_start2(3, (void *)0x80197000, 0xAF3C);
    kangei_ptr_tbl_init();
    sub_sub_step_up();
}
