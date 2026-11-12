/* Address: 0x80053280 */
/* Size: 228 bytes (57 instructions) */
/* Region: Game (leaf - the target has NO prologue, so no frame directive) */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x7c */
/* @CompileFlags: -Wo,-loopunroll,0 */
#include "types.h"

extern u8  byouki_kind;
extern u8  screen_intmode[];
extern u8  grp_tbl_end[];
extern u8  grp_tbl_end2[];
extern u8  get_grp_point_tbl0[];
extern volatile u8  girl0_flag2;
extern u8  girl0_spot2_cnt;
extern u8  girl0_spot3_cnt;
extern s16 girl0_syoushin;

s32 get_grp_point(u8 *a0, s16 *a1) {
    s32 sum;
    u8 *thr;
    u8 *end;

    if ((u32)a0[0] < 85) {
        return 0;
    }
    if ((byouki_kind & 0xF) != 0) {
        goto done;
    }

    end = grp_tbl_end;
    thr = get_grp_point_tbl0 + 1;
    a0 = screen_intmode;
    do {
        if ((u32)*(s16 *)(a0 + 0xFE) < (u32)thr[-1]) {
            goto done;
        }
        a0 += 4;
        thr++;
    } while (a0 < end);

    sum = girl0_flag2;
    sum += girl0_spot2_cnt;
    sum += girl0_spot3_cnt;
    end = grp_tbl_end2;
    a0 = grp_tbl_end - 0x1C;
    do {
        sum += a0[0x1D0];
        sum += a0[0x1D1];
        sum += a0[0x1D2];
        sum += a0[0x1D3];
        a0 += 4;
    } while (a0 != end);
    if (sum < 8) {
        goto done;
    }
    if (girl0_syoushin < 51) {
        *a1 = (s16)785;
    }
done:
    ;
}
