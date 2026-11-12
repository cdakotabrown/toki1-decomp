/* @RodataBase: 0x8015DCD0 */
/* @SymbolStart: 0x80156A60 */
/* @SymbolSize: 1280 */
/* Address: 0x80156A78 (real entry 0x80156A60) */
#include "types.h"

extern s32 taco_actor_count_owned(s32);
extern s32 taco_actor_count_alive(void);
extern void sub_sub_step_up(void);
extern void taco_actor_main(void);
extern void taco_emit_up(void);
extern void taco_substep_next(void);
extern void taco_phase_next(void);
extern void taco_emit_scatter(void);
extern void taco_actor_spawn_random(void);
extern void taco_emit_pos_x(void);
extern void taco_emit_neg_x(void);
extern void taco_emit_row(void);
extern void taco_emit_camera(void);
extern void taco_actor_pool_clear(void);

extern s32 taco_phase_cnt;
extern s32 taco_phase_no;
extern s32 taco_phase_frame;
extern s32 taco_substep_no;

void taco_scene_step(void)
{
    taco_phase_cnt = taco_phase_cnt + 1;
    taco_phase_frame = taco_phase_frame + 1;
    taco_actor_main();

    if (taco_phase_no < 12) {
        switch (taco_substep_no) {
        case 0:
            if (taco_phase_frame == 1) {
                taco_emit_up();
            }
            if (taco_actor_count_owned(14) == 0) {
                taco_substep_next();
            }
            break;
        case 1:
            taco_emit_scatter();
            taco_substep_next();
            break;
        case 2:
        case 3:
            if (taco_actor_count_owned(10) == 0) {
                taco_emit_scatter();
                taco_substep_next();
            }
            break;
        case 4:
            taco_actor_spawn_random();
            taco_substep_next();
            break;
        case 5:
        case 6:
            if (taco_actor_count_owned(11) == 0) {
                taco_actor_spawn_random();
                taco_substep_next();
            }
            break;
        case 7:
        case 8:
            taco_substep_next();
            break;
        case 9:
            taco_substep_next();
            taco_substep_no = 0;
            break;
        }
    }

    switch (taco_phase_no) {
    case 0:
    case 6:
        if (taco_phase_cnt >= 43) {
            if (taco_actor_count_owned(8) == 0) {
                taco_phase_next();
            }
        }
        switch (taco_phase_cnt) {
        case 2:
            taco_emit_pos_x();
            break;
        case 10:
            taco_emit_pos_x();
            break;
        case 18:
            taco_emit_pos_x();
            break;
        case 26:
            taco_emit_pos_x();
            break;
        case 34:
            taco_emit_pos_x();
            break;
        case 42:
            taco_emit_pos_x();
            break;
        }
        break;
    case 1:
    case 7:
        if (taco_phase_cnt >= 43) {
            if (taco_actor_count_owned(8) == 0) {
                taco_phase_next();
            }
        }
        switch (taco_phase_cnt) {
        case 2:
            taco_emit_neg_x();
            break;
        case 10:
            taco_emit_neg_x();
            break;
        case 18:
            taco_emit_neg_x();
            break;
        case 26:
            taco_emit_neg_x();
            break;
        case 34:
            taco_emit_neg_x();
            break;
        case 42:
            taco_emit_neg_x();
            break;
        }
        break;
    case 2:
    case 8:
        if (taco_phase_cnt >= 21) {
            if (taco_actor_count_owned(8) == 0) {
                taco_phase_next();
            }
        }
        if (taco_phase_cnt == 5) {
            taco_emit_row();
        }
        break;
    case 3:
    case 9:
        if (taco_phase_cnt >= 43) {
            if (taco_actor_count_owned(8) == 0) {
                taco_phase_next();
            }
        }
        switch (taco_phase_cnt) {
        case 2:
            taco_emit_pos_x();
            break;
        case 10:
            taco_emit_neg_x();
            break;
        case 18:
            taco_emit_pos_x();
            break;
        case 26:
            taco_emit_neg_x();
            break;
        case 34:
            taco_emit_pos_x();
            break;
        case 42:
            taco_emit_neg_x();
            break;
        }
        break;
    case 4:
    case 5:
    case 10:
    case 11:
        if (taco_phase_cnt >= 66) {
            if (taco_actor_count_owned(12) == 0) {
                taco_phase_next();
            }
        }
        switch (taco_phase_cnt) {
        case 5:
            taco_emit_camera();
            break;
        case 35:
            taco_emit_camera();
            break;
        case 65:
            taco_emit_camera();
            break;
        }
        break;
    case 12:
        if (taco_actor_count_owned(255) == 0) {
            taco_phase_next();
        }
        break;
    case 13:
        if (taco_actor_count_alive() == 0) {
            taco_actor_pool_clear();
            sub_sub_step_up();
        }
        break;
    }
}
