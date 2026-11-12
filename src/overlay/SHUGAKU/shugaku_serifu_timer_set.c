/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
#include "types.h"

extern s16 now_serifu_shugaku_in;
extern volatile s32 step_counter;
extern s32 k_wait_cnt;

extern void func_80138664(void);

void shugaku_serifu_timer_set(void)
{
    func_80138664();
    if ((s32)now_serifu_shugaku_in == 2) {
        if (step_counter == 1) {
            k_wait_cnt = 60;
        }
    }
    step_counter = step_counter + 1;
}
