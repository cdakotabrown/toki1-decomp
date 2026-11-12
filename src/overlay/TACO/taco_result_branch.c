/* Address: 0x801390E0 */
#include "types.h"

extern s32 taco_score;
extern s32 taco_rank_score[];

extern void goto_sub_step();
extern void disp_mode_set2();

void taco_result_branch(void)
{
    disp_mode_set2(0);

    if (taco_rank_score[2] < taco_score) {
        goto_sub_step(9);
    } else {
        goto_sub_step(5);
    }
}
