/* omimai_serifu_load - OMIMAI overlay, Address: 0x801325EC, Size: 68 bytes (17 insns) */
#include "types.h"

extern void cd_read_start2(s32, s32, s32);
extern void omimai_serifu_tbl_init(void);
extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);

void omimai_serifu_load(void)
{
    cd_read_start2(4, (s32)0x80197000, (s32)0xAF3F);
    omimai_serifu_tbl_init();
    device_cmd_send(1539);
    sub_sub_step_up();
}
