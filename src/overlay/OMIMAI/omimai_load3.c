/* omimai_load3 - OMIMAI overlay, Address: 0x801336F4, Size: 60 bytes (15 insns) */
#include "types.h"

extern void cd_read_start2(s32, s32, s32);
extern void omimai_serifu3_tbl_init(void);
extern void sub_sub_step_up(void);

void omimai_load3(void)
{
    cd_read_start2(6, (s32)0x80197000, (s32)0xAF33);
    omimai_serifu3_tbl_init();
    sub_sub_step_up();
}
