/* @SymbolSize: 304 */
/* Address: 0x8014EEE4 */
/* Size: 304 bytes (76 insns) */
#include "types.h"

extern u8 *taco_actor_rec;

extern void taco_rec_move(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g,
                          s32 h, s32 i);
extern void taco_cut_angle_set(void);
extern void taco_cut_seq(s32 a);

void taco_cut_run(s32 sel)
{
    if (sel < 14) {
        taco_rec_move(0, 0, -20, -30, -1, 0, 0, 50, 500);
    } else if (sel == 14) {
        taco_actor_rec[3534] = 1;
        taco_cut_angle_set();
    } else if (sel >= 15 && sel < 100) {
        if (taco_actor_rec[3534] == 1) {
            taco_cut_seq(0);
        }
        taco_rec_move(0, 0, 0, 0, 0, 0, 0, 50, 500);
    }
    if (sel >= 100 && sel < 114) {
        taco_rec_move(0, 0, 15, 30, 1, 0, 0, 50, 500);
    }
}
