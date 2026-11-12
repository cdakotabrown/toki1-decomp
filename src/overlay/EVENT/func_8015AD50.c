/* @CompileFlags: -cckr */
#include "types.h"
typedef struct { u8 pad0[6]; s16 f6; u8 pad8[2]; u16 f10; u8 pad12[3]; u8 f15; u8 pad16[20]; } Rec8015AD50;
extern Rec8015AD50 D_800EC220[];
func_8015AD50(d) short d; {
    D_800EC220[0].f6 += d;
    D_800EC220[1].f6 += d;
    D_800EC220[0].f10 -= d;
    D_800EC220[1].f10 -= d;
    D_800EC220[2].f10 += d;
    D_800EC220[3].f10 += d;
    D_800EC220[2].f15 -= d;
    D_800EC220[3].f15 -= d;
    return d;
}
