/* @PostProc: scheduler_normalizer --block-aware */
/* Address: 0x801520E0 */
/* Size: 220 bytes (55 insns) */
#include "types.h"

extern s32 taco_glyph_key_tbl[];

extern void taco_pose_keyframe_ease(s32, s32, s32, s32, s32);

void taco_glyph_row_disp(s32 x, s32 idx, s32 mode)
{
    s32 *p;
    s32 i;
    s32 cx;

    if (mode < 20) {
        i = 0;
        cx = x;
        p = &taco_glyph_key_tbl[idx];
        do {
            taco_pose_keyframe_ease(cx, *p, 1, 0, 0);
            i += 3;
            cx += 3;
            p++;
        } while (i != 21);
        return;
    }
    if (mode < 20) {
        return;
    }
    if (mode < 40) {
        i = 0;
        cx = x;
        p = &taco_glyph_key_tbl[idx];
        do {
            taco_pose_keyframe_ease(cx, *p, 2, 0, 0);
            i += 3;
            cx += 3;
            p++;
        } while (i != 21);
    }
}
