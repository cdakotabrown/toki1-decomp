/* @SymbolStart: 0x80138260 */
/* @SymbolSize: 200 */
#include "types.h"

extern s32 step_state;

extern s32 frame_counter;
extern u8 now_sub_sub_step;
extern char taco_main9_str0[];

extern void taco_play(void);
extern void play_end_set(void);
extern void func_80138328(void);
extern void taco_load_poll3(void);
extern void taco_main9_main2(void);
extern void taco_drop(s32, s32);
extern void taco_time_disp(void);
extern void taco_str_disp2(void *, s32, s32, s32);

void taco_case11_play_main(void)
{
    if ((u32)step_state < 96U) {
        if ((frame_counter & 0x10) != 0) {
            taco_str_disp2(taco_main9_str0, -64, 0, 2);
        }
    }
    switch (now_sub_sub_step) {
        case 0: func_80138328(); break;
        case 1: taco_load_poll3(); break;
        case 2: taco_main9_main2(); break;
    }
    taco_drop(1, 1);
    taco_time_disp();
    taco_play();
    play_end_set();
}
