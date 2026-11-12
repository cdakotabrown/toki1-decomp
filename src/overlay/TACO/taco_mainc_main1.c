/* @PostProc: scheduler_normalizer */
/* Address: 0x8014924C */
#include "types.h"

extern volatile u32 step_counter;
extern u8 taco_mainc_kind_tbl[];
extern u8 taco_mainc_ofs_tbl;
extern u8 taco_mainc_rad_tbl;
extern u8 taco_mainc_hp_tbl;

extern void taco_panel_anim();
extern void taco_anim_step();
extern void taco_panel();
extern void taco_slots_main();
extern void taco_actor_set();
extern s32 rand();
extern void sd_cmd_limit_send();
extern void taco_rec_move();
extern void sub_sub_step_up();

void
taco_mainc_main1(void)
{
    u8 *pv;
    s32 i;
    s32 v0;

    taco_panel_anim(3);
    taco_anim_step(6);
    taco_panel(4);
    taco_slots_main();

    pv = taco_mainc_kind_tbl;
    for (i = 0; i != 12; i++) {
        taco_actor_set(*pv, i, &taco_mainc_ofs_tbl, &taco_mainc_rad_tbl, &taco_mainc_hp_tbl);
        pv++;
    }

    if (step_counter % 240 == 0) {
        v0 = rand();
        if (v0 % 3 == 0) {
            sd_cmd_limit_send(0, 1280);
        }
    }
    if (step_counter < 300) {
        taco_rec_move(0, 0, -2, -40, 0, 0, 0, 60, 60);
    } else {
        sub_sub_step_up();
    }
    step_counter = step_counter + 1;
}
