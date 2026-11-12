/* geko_g_birth_cd_load -- GEKO overlay, Address: 0x80135C7C, Size: 56 bytes (14 insns) */
#include "types.h"

extern void cd_read_start2(s32 a0, s32 a1, u32 a2);
extern void geko_ptr_table_init2(void);
extern void sub_sub_step_up(void);

void geko_g_birth_cd_load(void)
{
    cd_read_start2(22, 0x801B0000, 0xAF0D);
    geko_ptr_table_init2();
    sub_sub_step_up();
}
