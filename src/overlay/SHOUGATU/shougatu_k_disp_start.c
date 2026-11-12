/* Address: 0x80141C90 */
/* Size: 76 bytes (19 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void k_disp_start();
extern void k_speed_set();
extern u8 get_k_speed(void);
extern s32 k_wait_cnt;
extern u8 shougatu_k_speed_wk;

void shougatu_k_disp_start(void)
{
    shougatu_k_speed_wk = get_k_speed();
    k_wait_cnt = 114;
    k_speed_set(30);
    k_disp_start(2);
    sub_sub_step_up();
}
