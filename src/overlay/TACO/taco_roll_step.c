/* @CompileEnv: IDO52_UGEN_TREG_ORDER_WINDOW=t6+t8+t9+t0+t1+t2+t3+t4+t5:2:2 */
/* Address: 0x8014F980   Size: 548 bytes (137 instructions) */
#include "types.h"

extern u8 *taco_actor_rec;
extern u8 taco_main16_hp_tbl[];
extern s32 step_counter;

extern s32 get_taco_percent(s32 a0, u8 *a1);
extern void taco_anim_disp(s32 a0);
extern void taco_list_chan_rand_set(s32 a0, s32 a1, s32 a2, s32 a3);
extern void sub_sub_step_up(void);

void taco_roll_step(void)
{
    s32 v0;
    u32 v1;

    v0 = get_taco_percent(11, taco_main16_hp_tbl);
    v1 = taco_actor_rec[2722];
    if (v0 != v1) {
        v0 = get_taco_percent(11, taco_main16_hp_tbl);
        taco_actor_rec[2722] = v0;
        taco_actor_rec[2854] = 1;
        v1 = taco_actor_rec[2722];
    }
    if (v1 < 32) {
        taco_anim_disp(6);
        if (taco_actor_rec[2854] != 0) {
            taco_list_chan_rand_set((s32)0x801A7000, 10, 256, 495);
            taco_actor_rec[2854] = 0;
        }
    } else if (v1 >= 32 && v1 < 48) {
        taco_anim_disp(5);
        if (taco_actor_rec[2854] != 0) {
            taco_list_chan_rand_set((s32)0x801A7000, 10, 256, 495);
            taco_actor_rec[2854] = 0;
        }
    } else if (v1 >= 48 && v1 < 64) {
        taco_anim_disp(4);
        if (taco_actor_rec[2854] != 0) {
            taco_list_chan_rand_set((s32)0x801A7000, 10, 256, 495);
            taco_actor_rec[2854] = 0;
        }
    } else if (v1 >= 64 && v1 < 72) {
        taco_anim_disp(3);
        if (taco_actor_rec[2854] != 0) {
            taco_list_chan_rand_set((s32)0x801A7000, 10, 256, 495);
            taco_actor_rec[2854] = 0;
        }
    } else if (v1 >= 72 && v1 < 100) {
        taco_anim_disp(2);
        if (taco_actor_rec[2854] != 0) {
            taco_list_chan_rand_set((s32)0x801A7000, 10, 256, 495);
            taco_actor_rec[2854] = 0;
        }
    } else {
        step_counter = 0;
        sub_sub_step_up();
    }
}
