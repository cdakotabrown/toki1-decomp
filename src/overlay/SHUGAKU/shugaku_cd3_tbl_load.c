/* shugaku_cd3_tbl_load - SHUGAKU overlay, Address: 0x80133720, Size: 56 bytes (14 insns) */
#include "types.h"

extern void cd_read_start2(s32, s32, s32);
extern void shugaku_tbl_init(void);
extern void sub_sub_step_up(void);

void shugaku_cd3_tbl_load(void)
{
    cd_read_start2(3, 0x801B0000, 0xAF43);
    shugaku_tbl_init();
    sub_sub_step_up();
}
