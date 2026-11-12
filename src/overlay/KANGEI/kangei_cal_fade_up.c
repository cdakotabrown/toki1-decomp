/* @PostProc: swap_registers v1:v0 --unidirectional */
/* @PostProc: scheduler_normalizer */
#include "types.h"

extern void fade_up_main(void);

extern u8 now_girl;
extern u8 bustup2_cw_bright;
extern u8 cal_cw_disp;
extern u8 cal_cw_bright;

void kangei_cal_fade_up(void)
{
    u8 cur_char;

    fade_up_main();
    cur_char = now_girl;
    if (cur_char == 10) {
        goto body;
    }
    if (cur_char == 3) {
    body:
        cal_cw_disp = cal_cw_disp | 0x80;
        cal_cw_bright = bustup2_cw_bright;
    }
}
