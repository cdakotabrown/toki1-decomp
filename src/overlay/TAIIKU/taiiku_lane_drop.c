/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=1 IDO52_PRECOLOR_WEB=v0:0 */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: patch_insn_reg_field 0x44:rt:a1:a0 0x54:rt:a0:a1 0x60:rs:a0:a1 0x68:rd:a1:a0 0x7c:rs:a1:a0 0xa8:rs:a1:a0 0xd4:rs:a1:a0 0x104:rs:a1:a0 */
/* Address: 0x80148814 */
/* Size: 316 bytes (79 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 Vblnk_Timer(void);

extern u8 taiiku_spoon_rank[];
extern s16 cal_cw_x;
extern s16 cw97_x;
extern s16 cw98_x;
extern CLASS_W class_w[];

s32 taiiku_lane_drop(void)
{
    s32 sel;
    s32 i;
    s32 ret;
    u8 *p;

    ret = Vblnk_Timer();
    if ((u32)ret >= 46U) {
    class_w[104].frame = 60;
    class_w[104].y.half[1] = -8;

    p = taiiku_spoon_rank;
    i = 0;
    do {
        if (*p == 1) {
            sel = i;
        }
        i++;
        p++;
    } while (i < 4);

    if (sel < 0) {
        cal_cw_x = cal_cw_x + 48;
        class_w[100].x.half[1] = class_w[100].x.half[1] + 48;
    }
    if (sel <= 0) {
        cw97_x = cw97_x + 48;
        class_w[101].x.half[1] = class_w[101].x.half[1] + 48;
    }
    if (sel < 2) {
        cw98_x = cw98_x + 48;
        class_w[102].x.half[1] = class_w[102].x.half[1] + 48;
    }
    if (sel < 3) {
        class_w[99].x.half[1] = class_w[99].x.half[1] + 48;
        class_w[103].x.half[1] = class_w[103].x.half[1] + 48;
    }
    ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}
