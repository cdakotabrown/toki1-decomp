/* @SymbolStart: 0x8014526C */
/* @SymbolSize: 136 */
/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: patch_stack_complete --old 8 --new 32 */
/* @PostProc: remap_stack_slots --from 0 --to 20 --from 2 --to 22 --from 4 --to 24 --from 6 --to 26 */
/* Address: 0x80145274 (real start 0x8014526C; cross-prologue head computes the */
#include "types.h"
#include "tokimeki/class_w.h"

typedef struct { s32 a, b; } S2;

extern S2 taiiku_result_frame_add2_tbl;
extern s16 cal_cw_frame;
extern s16 cw97_frame;
extern s16 cw98_frame;
extern CLASS_W class_w[];

void taiiku_result_frame_step(void) {
    union { S2 s; s16 h[4]; } local;

    local.s = taiiku_result_frame_add2_tbl;
    cal_cw_frame += local.h[0];
    cw97_frame += local.h[1];
    cw98_frame += local.h[2];
    class_w[99].frame += local.h[3];
}
