/* @SymbolStart: 0x80148080 */
/* @SymbolSize: 212 */
/* Address: 0x80148088 */
#include "types.h"

extern u32 step_state;

extern u8 taco_maina_str0[];
extern s32 frame_counter;
extern u8 now_sub_sub_step;

extern void taco_play();
extern void play_end_set();
extern void taco_time_disp();
extern void taco_str_disp2();
extern void taco_maina_init0();
extern void taco_maina_timeline_step();
extern void taco_maina_actor_face_step();
extern void taco_maina_band_fade_step();

void taco_maina(void)
{
    u32 t6 = step_state;

    if (t6 < 96U && (frame_counter & 0x10)) {
        taco_str_disp2(taco_maina_str0, -48, 0, 2);
    }

    switch (now_sub_sub_step) {
    case 0:
        taco_maina_init0();
        break;
    case 1:
        taco_maina_timeline_step();
        break;
    case 2:
        taco_maina_actor_face_step();
        break;
    case 3:
        taco_maina_band_fade_step();
        break;
    }

    taco_time_disp();
    taco_play();
    play_end_set();
}
