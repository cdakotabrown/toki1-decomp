/* @SymbolSize: 296 */
/* Address: 0x8014EAF4 */
/* Size: 296 bytes (74 insns) */
#include "types.h"

extern u8 *taco_actor_rec;

extern void taco_rec_move(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g,
                          s32 h, s32 i);
extern void taco_stage_angle_set(void);
extern void taco_stage_seq(s32 a);

void taco_stage_run(s32 sel)
{
    if (sel < 14) {
        taco_rec_move(0, 0, -20, -30, 0, 0, 0, 50, 500);
    } else if (sel == 14) {
        taco_actor_rec[3398] = 1;
        taco_stage_angle_set();
    } else if (sel >= 15 && sel < 65) {
        if (taco_actor_rec[3398] == 1) {
            taco_stage_seq(0);
        }
        taco_rec_move(0, 0, 0, 0, 0, 0, 0, 50, 500);
    }
    if (sel >= 65 && sel < 80) {
        taco_rec_move(0, 0, 15, 30, 0, 0, 0, 50, 500);
    }
}
