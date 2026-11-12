/* kangei_cd_read - KANGEI overlay, Address: 0x80133CC0, Size: 60 bytes (15 insns) */
#include "types.h"

extern void cd_read_start2(s32, void *, s32);
extern void kangei_ptr_tbl2_init(void);
extern void sub_sub_step_up(void);

void
kangei_cd_read(void)
{
    cd_read_start2(59, (void *)0x80197000, 0xAE60);
    kangei_ptr_tbl2_init();
    sub_sub_step_up();
}
