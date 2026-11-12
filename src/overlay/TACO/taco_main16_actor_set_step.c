/* Address: 0x8014A204 */
/* Size: 348 bytes (87 insns) */
#include "types.h"

extern s32 step_counter;
extern u8 *taco_actor_rec;
extern u8 taco_round_no;
extern u8 taco_main16_kind_tbl[];
extern u8 taco_main16_rad_tbl[];
extern u8 taco_main16_hp_tbl[];
extern u8 taco_main16_ofs_tbl[];

extern void sub_sub_step_up(void);
extern void sd_cmd_limit_send(s32, s32);
extern s32 get_taco_percent(s32, u8 *);
extern void taco_actor_set2(s32, s32, u8 *, u8 *, u8 *, s32);
extern void taco_light_set(void);
extern void taco_row_rand_step(void);
extern void taco_chime_play(void);

void taco_main16_actor_set_step(void)
{
    s32 i;

    for (i = 0; i != 11; i++) {
        taco_actor_set2(taco_main16_kind_tbl[i], i, taco_main16_ofs_tbl, taco_main16_rad_tbl, taco_main16_hp_tbl,
                      step_counter);
    }

    taco_light_set();
    taco_row_rand_step();

    if ((u32)step_counter % 180U == 0) {
        sd_cmd_limit_send(0, 1280);
    }
    taco_chime_play();

    if (get_taco_percent(11, taco_main16_hp_tbl) >= 85 && taco_actor_rec[2309] == 0 &&
        taco_actor_rec[2717] == 0 && taco_actor_rec[2853] == 0) {
        taco_round_no = taco_round_no + 1;
        step_counter = 0;
        sub_sub_step_up();
    }

    step_counter = step_counter + 1;
}
