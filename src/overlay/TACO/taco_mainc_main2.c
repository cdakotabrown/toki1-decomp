/* Address: 0x801493A4 */
/* Size: 356 bytes (89 insns) */
#include "types.h"

extern s32 step_counter;
extern u8 *taco_actor_rec;
extern u8 taco_round_no;
extern u8 taco_mainc_kind_tbl[];
extern u8 taco_mainc_rad_tbl[];
extern u8 taco_mainc_hp_tbl[];
extern u8 taco_mainc_ofs_tbl[];

extern void sub_sub_step_up(void);
extern s32 rand(void);
extern void sd_cmd_limit_send(s32, s32);
extern s32 get_taco_percent(s32, u8 *);
extern void taco_actor_set(s32, s32, u8 *, u8 *, u8 *);
extern void taco_light_pulse12(void);
extern void taco_panel_main(void);
extern void taco_list_chan_set(s32, s32);

void taco_mainc_main2(void)
{
    s32 i;

    for (i = 0; i != 12; i++) {
        taco_actor_set(taco_mainc_kind_tbl[i], i, taco_mainc_ofs_tbl, taco_mainc_rad_tbl, taco_mainc_hp_tbl);
    }

    taco_light_pulse12();
    taco_panel_main();

    if ((u32)step_counter % 240U == 0) {
        if (rand() % 3 == 0) {
            sd_cmd_limit_send(0, 1280);
        }
    }

    if (get_taco_percent(12, taco_mainc_hp_tbl) >= 85 && taco_actor_rec[2309] == 0 &&
        taco_actor_rec[2445] == 0 && taco_actor_rec[2309] == 0) {
        taco_list_chan_set(0x80196000, 9);
        taco_round_no = taco_round_no + 1;
        sub_sub_step_up();
    }

    step_counter = step_counter + 1;
}
