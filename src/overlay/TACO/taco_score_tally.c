/* @CompileEnv: IDO52_FORCE_WEB_COLOR=4:a0 */
/* @SymbolStart: 0x80138B0C */
/* @SymbolSize: 452 */
/* Address: 0x80138B6C */
/* Size: 356 bytes (89 insns) */
#include "types.h"

extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;
extern u8 taco_round_no;
extern s32 taco_tally_base;
extern u8 *taco_actor_rec;
extern s16 taco_time_left;
extern s32 taco_score;
extern s32 taco_tally_power;
extern s32 taco_tally_rank;
extern s32 taco_tally_time;
extern s32 taco_tally_total;

extern void sub_sub_step_up(void);
extern void device_cmd_send(s32 a0);
extern void all_object_disp_off(void);
extern void taco_objects_init(void);

void taco_score_tally(void)
{
    s32 mode;
    s32 items;
    s32 combo;
    s32 rank;

    mode = taco_round_no;
    back_clear_g = 0;
    back_clear_r = 0;
    taco_tally_base = taco_score += mode * 1000000;
    back_clear_b = 0;

    combo = 0;
    if ((u32)mode >= 3) {
        if (taco_actor_rec[133] == 255) {
            items = 500000;
        } else {
            items = taco_actor_rec[133] * 1000;
        }
        combo = taco_time_left * 300;
    } else {
        items = taco_actor_rec[133] * 100;
    }

    if (*(s16 *)(taco_actor_rec + 128) != 0) {
        rank = 500000;
    } else if (*(s16 *)(taco_actor_rec + 126) != 0) {
        rank = 250000;
    } else {
        if (*(s16 *)(taco_actor_rec + 124) != 0) {
            rank = 100000;
        } else {
            rank = 0;
        }
    }

    taco_tally_total = taco_score + items + rank + combo;
    taco_tally_rank = rank;
    taco_tally_power = items;
    taco_tally_time = combo;
    device_cmd_send(36);
    all_object_disp_off();
    taco_objects_init();
    sub_sub_step_up();
}
