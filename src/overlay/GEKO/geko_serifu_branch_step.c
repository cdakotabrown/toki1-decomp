/* geko_serifu_branch_step -- GEKO overlay, Address: 0x8013BF40, Size: 92 bytes (23 insns) */
#include "types.h"

extern void sub_sub_step_up(void);

extern s8 ev_flag4f;
extern u8 now_sub_sub_step;
extern s32 serifu_no;

void geko_serifu_branch_step(void)
{
    s8 val;
    sub_sub_step_up();
    val = 2;
    if (serifu_no != 0) {
        now_sub_sub_step = now_sub_sub_step + 20;
        ev_flag4f = 1;
    } else {
        ev_flag4f = val;
    }
}
