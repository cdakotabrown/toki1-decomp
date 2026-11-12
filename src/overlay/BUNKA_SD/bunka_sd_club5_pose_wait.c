/* @CompileEnv: IDO52_COPYPROP_SITE=lod:2:deny IDO52_FORCE_WEB_COLOR=0:v1,8:v0,6:a0 */
#include "types.h"

extern void bunka_sd_head_timer(void);
extern void bunka_sd_pose_set(void);
extern void sub_sub_step_up(void);

extern volatile s32 step_counter;
extern u8 now_year;

void bunka_sd_club5_pose_wait(void)
{
    s32 counter;
    s32 t;

    bunka_sd_head_timer();
    bunka_sd_pose_set();
    counter = step_counter + 1;
    t = counter + 0x90000000;
    if (t == 250) {
        if (now_year != 96) {
            step_counter = counter;
            sub_sub_step_up();
            counter = step_counter;
            t = counter + 0x90000000;
        }
    }
    step_counter = counter;
    if (t == 208) {
        if (now_year == 96) {
            sub_sub_step_up();
        }
    }
}
