/* shugaku_cd69_tbl3_load - SHUGAKU overlay, Address: 0x80134610, Size: 56 bytes (14 insns) */
#include "types.h"

extern void cd_read_start2(s32, s32, s32);
extern void shugaku_tbl_init3(void);
extern void sub_sub_step_up(void);

void shugaku_cd69_tbl3_load(void)
{
    cd_read_start2(0x45, 0x801B0000, 0x8902);
    shugaku_tbl_init3();
    sub_sub_step_up();
}
