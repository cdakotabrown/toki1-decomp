/* @SymbolSize: 1000 */
/* @PostProc: patch_insn_reg_field 0x138:rd:a1:at 0x190:rd:a1:at 0x1e8:rd:a1:at 0x250:rd:a1:at 0x2a8:rd:a1:at 0x13c:rt:a1:at 0x140:rt:a1:at 0x194:rt:a1:at 0x198:rt:a1:at 0x1ec:rt:a1:at 0x1f0:rt:a1:at 0x1f8:rt:a1:at 0x200:rt:a1:at 0x254:rt:a1:at 0x258:rt:a1:at 0x2ac:rt:a1:at 0x2b0:rt:a1:at */
/* Address: 0x8014D500 */
/* Size: 1000 bytes (250 insns) */
#include "types.h"

extern void taco_rec_move(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g,
                          s32 h, s32 i);

void taco_offset_set(s32 sel, s32 val)
{
    s32 *force;

    force = &val;
    (void)force;
    if (sel < 5) {
        taco_rec_move(0, val * 150, 0, 0, 0, 0, 0, 50, 50);
        return;
    }
    if (sel >= 5 && sel < 10) {
        taco_rec_move(0, val * 120, -30, -200, 0, 0, 0, 50, 50);
        return;
    }
    if (sel >= 10 && sel < 15) {
        taco_rec_move(0, val * 80, -30, -200, 0, 0, 0, 50, 50);
        return;
    }
    if (sel >= 15 && sel < 20) {
        taco_rec_move(0, (-val) * 80, -30, -200, 0, 0, 0, 50, 50);
        return;
    }
    if (sel >= 20 && sel < 25) {
        taco_rec_move(0, (-val) * 120, -30, -200, 0, 0, 0, 50, 50);
        return;
    }
    if (sel >= 25 && sel < 35) {
        taco_rec_move(0, (-val) * 150, 0, 0, 0, 0, 0, 50, 50);
        return;
    }
    if (sel >= 35 && sel < 40) {
        taco_rec_move(0, (-val) * 120, 30, 200, 0, 0, 0, 50, 50);
        return;
    }
    if (sel >= 40 && sel < 45) {
        taco_rec_move(0, (-val) * 80, 30, 200, 0, 0, 0, 50, 50);
        return;
    }
    if (sel >= 45 && sel < 50) {
        taco_rec_move(0, val * 80, 30, 200, 0, 0, 0, 50, 50);
        return;
    }
    if (sel >= 50 && sel < 55) {
        taco_rec_move(0, val * 120, 30, 200, 0, 0, 0, 50, 50);
        return;
    }
    if (sel >= 55 && sel < 60) {
        taco_rec_move(0, val * 150, 0, 0, 0, 0, 0, 50, 50);
    }
}
