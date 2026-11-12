/* omimai_serifu2_load - OMIMAI overlay, Address: 0x80132DC0, Size: 56 bytes (14 insns) */
#include "types.h"

extern void cd_read_start2(s32, s32, s32);
extern void omimai_serifu2_tbl_init(void);
extern void sub_sub_step_up(void);

void omimai_serifu2_load(void)
{
    cd_read_start2(3, (s32)0x801B0000, (s32)0xAF43);
    omimai_serifu2_tbl_init();
    sub_sub_step_up();
}
