/* @PostProc: patch_stack_complete --old 144 --new 160 */
/* @PostProc: remap_stack_slots --from 32 --to 44 */
/* Address: 0x8013E400 */
/* Size: 224 bytes (56 instructions) */
#include "types.h"

typedef s32 (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[28];
} JumpTable;

extern u8 now_sub_sub_step;
extern volatile u8 cal_cw_bright;
extern s16 cal_cw_tick;
extern volatile s16 cal_cw_anim;
extern volatile s16 cal_cw_frame;
extern u8 fade_bri;
extern JumpTable shougatu_main2_tbl;

s32 shougatu_case2_cal_cw_step(void)
{
    JumpTable table;
    s32 v0;

    table = shougatu_main2_tbl;

    v0 = table.ptrs[now_sub_sub_step](128);

    cal_cw_bright = fade_bri;

    if (cal_cw_frame != 0) {
        return;
    }
    if (cal_cw_tick != 0) {
        return;
    }

    v0 = cal_cw_anim + 1;
    v0 = (v0 << 16) >> 16;
    cal_cw_anim = (s16)v0;
    if (v0 < 3) {
        return;
    }

    cal_cw_anim = 0;
    return 0;
}
