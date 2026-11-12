/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --from 31 --to 43 */
/* Address: 0x80141BB8 */
/* Size: 176 bytes (44 instructions) */
#include "types.h"

extern s32 get_now_k(void);
extern void shougatu_serifuev_run2(void);
extern s32 step_counter;
extern u8 cal_cw_mode;
extern s32 serifuev_end_flag;

void shougatu_serifuev_step(void)
{
    u8 flag;
    s32 count;

    flag = 1;
    if ((u32)get_now_k() >= 2U) {
        step_counter = step_counter + 1;
    }
    count = step_counter;
    if ((u32)count >= 115U) {
        if ((u32)count % 60U == 6U) {
            goto call;
        }
    }
    if ((cal_cw_mode & 1) != 0) {
        goto call;
    }
    flag = 0;
call:
    shougatu_serifuev_run2();
    if (flag != 0) {
        return;
    }
    if (serifuev_end_flag == 0) {
        return;
    }
    cal_cw_mode = 0;
}
