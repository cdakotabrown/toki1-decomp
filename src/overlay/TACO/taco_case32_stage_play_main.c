/* @SymbolStart: 0x80149F60 */
/* @RodataBase: 0x8015DC4C */
/* @SymbolSize: 224 */
/* Address: 0x80149F68 */
#include "types.h"

extern s32 step_state;

extern s32 frame_counter;
extern u8 now_sub_sub_step;
extern char taco_main16_str0[];

extern void taco_play(void);
extern void play_end_set(void);
extern void taco_time_disp(void);
extern void taco_str_disp2(void *, s32, s32, s32);
extern void taco_main16_init0(void);
extern void taco_main16_rows_chime_step(void);
extern void taco_main16_actor_set_step(void);
extern void taco_main16_anim_fade_step(void);
extern void taco_main16_drift_step(void);

void taco_case32_stage_play_main(void)
{
    s32 t6_in = step_state;

    u8 sel;

    if ((u32)t6_in < 96U) {
        if ((frame_counter & 0x10) != 0) {
            taco_str_disp2(taco_main16_str0, -48, 0, 2);
        }
    }
    sel = now_sub_sub_step;
    switch (sel) {
        case 0: taco_main16_init0(); break;
        case 1: taco_main16_rows_chime_step(); break;
        case 2: taco_main16_actor_set_step(); break;
        case 3: taco_main16_anim_fade_step(); break;
        case 4: taco_main16_drift_step(); break;
    }
    taco_time_disp();
    taco_play();
    play_end_set();
}
