/* @SymbolStart: 0x8014DF20 */
/* @SymbolSize: 248 */
/* Address: 0x8014DF38 (true entry 0x8014DF20 -- cross-prologue hoisted head) */
/* Size: 248 bytes (62 instructions incl. the 6-insn head func_8014DF20) */
#include "types.h"
#include "tokimeki/libgs_types.h"
extern LIBGS_COORD2 taco_coord[];


extern void taco_rec_move();

void taco_phase_pan(s32 sel)
{
    s32 v;

    v = (-16800 - taco_coord[0].coord.t[2]) / 15;
    if (sel < 5) {
        taco_rec_move(0, 100, 0, v, 0, 0, 0, 50, 50);
    }
    if (sel >= 5 && sel < 15) {
        taco_rec_move(0, -100, 0, v, 0, 0, 0, 50, 50);
    }
    if (sel >= 15 && sel < 20) {
        taco_rec_move(0, 100, 0, v, 0, 0, 0, 50, 50);
    }
}
