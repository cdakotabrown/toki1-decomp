/* @SymbolStart: 0x80148FB0 */
/* @SymbolSize: 212 */
#include "types.h"

extern s32 step_state;

extern s32 frame_counter;
extern u8 now_sub_sub_step;
extern u8 taco_mainc_str0[];

extern void taco_str_disp2(void *a0, s32 a1, s32 a2, s32 a3);
extern void taco_mainc_init0(void);
extern void taco_mainc_main1(void);
extern void taco_mainc_main2(void);
extern void taco_mainc_main3(void);
extern void taco_time_disp(void);
extern void taco_play(void);
extern void play_end_set(void);

void taco_mainc(void)
{
    s32 sel = step_state;

    if ((u32)sel < 96U && (frame_counter & 0x10) != 0) {
        taco_str_disp2(taco_mainc_str0, -48, 0, 2);
    }

    switch (now_sub_sub_step) {
    case 0:
        taco_mainc_init0();
        break;
    case 1:
        taco_mainc_main1();
        break;
    case 2:
        taco_mainc_main2();
        break;
    case 3:
        taco_mainc_main3();
        break;
    }

    taco_time_disp();
    taco_play();
    play_end_set();
}
