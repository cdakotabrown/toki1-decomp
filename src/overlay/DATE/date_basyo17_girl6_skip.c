/* @SymbolStart: 0x8013BE54 */
/* @SymbolSize: 220 */
/* @CompileFlags: -cckr */
/* @PostProc: swap_instructions --offset1 0x90 --offset2 0xA0 */
/* @PostProc: swap_instructions --offset1 0x98 --offset2 0xA8 */
#include "types.h"

extern u8  now_girl;
extern u8  now_sub_sub_step;
extern s16 para_un;
extern s16 now_serifu;
extern s16 kao[];
extern u16 kao2[];
extern s16 imp[];
extern s16 ans[];

void sub_sub_step_up();
void goto_sub_sub_step();

void date_basyo17_girl6_skip()
{
    if (now_girl != 6) {
        sub_sub_step_up();
        now_sub_sub_step += 8;
    } else if (para_un >= 150) {
        now_serifu += 2;
        goto_sub_sub_step(45);
    } else {
        ans[0] = imp[2] = imp[0] = imp[3] = 2;
        kao[0] = kao[1] = kao[2] = 3;
        kao2[0] = kao2[1] = kao2[2] = 3;
        sub_sub_step_up();
    }
}
