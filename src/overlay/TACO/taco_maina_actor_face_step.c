/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x70 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xd0 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=1 */
/* @SymbolSize: 308 */
/* Address: 0x80148380 */
#include "types.h"

extern u8 taco_main2_kind_tbl[];
extern u8 taco_main2_rad_tbl[];
extern u8 taco_main2_hp_tbl[];
extern u8 taco_main2_ofs_tbl[];
extern s32 taco_obj_1_attribute;
extern s32 taco_obj_2_attribute;
extern s32 taco_obj_3_attribute;
extern s32 taco_obj_31_attribute;
extern s32 step_counter;
extern u8 taco_round_no;

extern void taco_drop();
extern void taco_actor_face_set();
extern void taco_light_pulse11();
extern void taco_roster_step();
extern s32 get_taco_percent();
extern void sd_cmd_limit_send();
extern void sub_sub_step_up();

void
taco_maina_actor_face_step(void)
{
    s32 *p;
    s32 v0;
    s32 i;
    u8 *q;

    taco_drop(1, 1);
    q = taco_main2_kind_tbl;
    for (i = 0; i < 11; i++) {
        taco_actor_face_set(*q, i, taco_main2_ofs_tbl, taco_main2_rad_tbl, taco_main2_hp_tbl);
        q++;
    }
    taco_light_pulse11();
    taco_roster_step(step_counter);
    v0 = get_taco_percent(11, taco_main2_hp_tbl);
    if (v0 >= 80) {
        taco_obj_1_attribute = 0x40000000;
        taco_obj_2_attribute = 0x40000000;
        p = &taco_obj_3_attribute;
        do {
            p += 16;
            *(p - 12) = 0x40000000;
            *(p - 8) = 0x40000000;
            *(p - 4) = 0x40000000;
            *(p - 16) = 0x40000000;
        } while (p != &taco_obj_31_attribute);
        sd_cmd_limit_send(0, 1285);
        taco_round_no = taco_round_no + 1;
        sub_sub_step_up();
    }
    step_counter = step_counter + 1;
}
