/* @SymbolSize: 800 */
/* Address: 0x801512EC */
/* Size: 800 bytes (200 insns) */
#include "types.h"

extern void taco_rec_move(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g,
                          s32 h, s32 i);

void taco_frame_pos_set(s32 sel)
{
    if (sel < 10) {
        taco_rec_move(0, 18, 0, 0, 0, 0, 0, 200, 200);
    } else if (sel >= 10 && sel < 20) {
        taco_rec_move(0, 18, -12, 0, 0, 0, 0, 200, 200);
    } else if (sel >= 20 && sel < 30) {
        taco_rec_move(0, 18, -13, 0, 0, 0, 0, 200, 200);
    } else if (sel >= 32 && sel < 42) {
        taco_rec_move(0, -18, 20, 0, 0, 0, 0, 200, 200);
    } else if (sel >= 42 && sel < 52) {
        taco_rec_move(0, -18, 15, 0, 0, 0, 0, 200, 200);
    } else if (sel >= 52 && sel < 72) {
        taco_rec_move(0, -18, 0, 0, 0, 0, 0, 200, 200);
    } else if (sel >= 72 && sel < 82) {
        taco_rec_move(0, -18, -12, 0, 0, 0, 0, 200, 200);
    } else if (sel >= 82 && sel < 92) {
        taco_rec_move(0, -18, -13, 0, 0, 0, 0, 200, 200);
    } else if (sel >= 94 && sel < 104) {
        taco_rec_move(0, 18, 20, 0, 0, 0, 0, 200, 200);
    } else if (sel >= 104 && sel < 114) {
        taco_rec_move(0, 18, 15, 0, 0, 0, 0, 200, 200);
    } else if (sel >= 114 && sel < 124) {
        taco_rec_move(0, 18, 0, 0, 0, 0, 0, 200, 200);
    }
}
