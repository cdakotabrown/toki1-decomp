/* geko_girl6_ev1_cd_load -- GEKO overlay, Address: 0x8013DC20, Size: 56 bytes (14 insns) */
#include "types.h"

extern void cd_read_start2(s32 a0, s32 a1, u32 a2);
extern void geko_enq2_res_init(void);
extern void sub_sub_step_up(void);

void geko_girl6_ev1_cd_load(void)
{
    cd_read_start2(61, 0x801B0000, 0x8947);
    geko_enq2_res_init();
    sub_sub_step_up();
}
