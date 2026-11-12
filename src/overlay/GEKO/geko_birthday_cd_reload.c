/* geko_birthday_cd_reload -- GEKO overlay, Address: 0x80134738, Size: 56 bytes (14 insns) */
#include "types.h"

extern void cd_read_start2(s32 a0, s32 a1, u32 a2);
extern void geko_ptr_table_init1(void);
extern void sub_sub_step_up(void);

void geko_birthday_cd_reload(void)
{
    cd_read_start2(3, 0x801B0000, 0xAF43);
    geko_ptr_table_init1();
    sub_sub_step_up();
}
