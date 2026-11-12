/* Address: 0x801471E0 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

extern u8 p_sei;
extern u8 etc_week_main1_1_str0;
extern u8 etc_week_main1_1_str1;

extern void sub_sub_step_up();
extern void text_sys_init();
extern void k_disp_start();
extern void gnsx();
extern void sndisp();
extern void basyo_init();
extern void etc_state_init();
extern s32 score_grade_calc();

void etc_week_main1_1(void)
{
    s32 result;

    text_sys_init();
    basyo_init(&etc_week_main1_1_str0);
    gnsx(&p_sei);
    k_disp_start(2);
    sndisp(&etc_week_main1_1_str1, 0, 31);
    result = score_grade_calc();
    if (result < 2) {
        etc_state_init(1);
    } else {
        etc_state_init(0);
    }
    sub_sub_step_up();
}
