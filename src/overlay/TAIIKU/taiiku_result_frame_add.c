/* @SymbolStart: 0x80140564 */
/* @SymbolSize: 208 */
/* @CompileFlags: -Wo,-notail */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x48 */
/* Address: 0x80140564 (real start; cross-prologue head computes the */
#include "types.h"

typedef struct { s32 a, b; } S2;
typedef struct { s16 v; u8 pad[66]; } Rec68;

extern S2 taiiku_result_frame_add_tbl;
extern Rec68 cal_cw_frame[4];
extern u8 taiiku_tama_rtick;

void taiiku_result_frame_add(void) {
    s32 pad;
    union { S2 s; s16 h[4]; } local;

    local.s = taiiku_result_frame_add_tbl;
    cal_cw_frame[0].v += local.h[0];
    cal_cw_frame[1].v += local.h[1];
    cal_cw_frame[2].v += local.h[2];
    cal_cw_frame[3].v += local.h[3];
}

void taiiku_result_frame_blink(void) {
    if (taiiku_tama_rtick >= 15U) {
        cal_cw_frame[1].v ^= 1;
        cal_cw_frame[2].v ^= 1;
        taiiku_tama_rtick = 0;
    }
}
