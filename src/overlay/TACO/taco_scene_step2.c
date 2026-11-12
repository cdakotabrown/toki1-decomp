/* @RodataBase: 0x8015DF20 */
/* @SymbolStart: 0x801590A0 */
/* @SymbolSize: 464 */
/* Address: 0x801590A8 (real entry 0x801590A0) */
#include "types.h"

extern s32 taco_actor_count_owned(s32);
extern s32 taco_actor_count_alive(void);
extern void sub_sub_step_up(void);
extern void taco_actor_main(void);
extern void taco_effect_rand(void);
extern void taco_effect_angle(s32);
extern void taco_ring_spawn(s32);
extern void taco_phase_next(void);
extern void taco_actor_pool_clear(void);

extern s32 taco_phase_cnt;
extern s32 taco_phase_no;

void taco_scene_step2(void)
{
    taco_phase_cnt = taco_phase_cnt + 1;
    taco_actor_main();

    if (taco_phase_cnt % 30 == 0) {
        if (taco_phase_no < 4) {
            if (taco_actor_count_owned(2) < 10) {
                taco_effect_rand();
            }
        } else if (taco_phase_no < 6) {
            if (taco_actor_count_owned(2) < 3) {
                taco_effect_rand();
            }
        }
    }

    switch (taco_phase_no) {
    case 0:
        taco_effect_angle(taco_phase_no);
        taco_phase_next();
        break;
    case 1:
    case 2:
    case 3:
        if (taco_actor_count_owned(3) == 0) {
            taco_effect_angle(taco_phase_no);
            taco_phase_next();
        }
        break;
    case 4:
        if (taco_actor_count_owned(3) == 0) {
            taco_phase_next();
        }
        break;
    case 5:
        taco_phase_next();
        break;
    case 6:
        taco_phase_next();
        break;
    case 7:
    case 8:
    case 9:
    case 10:
        if (taco_actor_count_alive() == 0) {
            taco_ring_spawn(taco_phase_no - 3);
            taco_phase_next();
        }
        break;
    case 11:
        if (taco_actor_count_owned(255) == 0) {
            taco_phase_next();
        }
        break;
    case 12:
        if (taco_actor_count_alive() == 0) {
            taco_actor_pool_clear();
            sub_sub_step_up();
        }
        break;
    }
}
