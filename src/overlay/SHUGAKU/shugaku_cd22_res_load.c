/* shugaku_cd22_res_load - SHUGAKU overlay, Address: 0x801326C0, Size: 56 bytes (14 insns) */
#include "types.h"

void
shugaku_cd22_res_load(void)
{
    cd_read_start2(22, (s32)0x801B0000, 0xAF0D);
    shugaku_res_init();
    sub_sub_step_up();
}
