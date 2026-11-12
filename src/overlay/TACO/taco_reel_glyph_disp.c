/* @PostProc: scheduler_normalizer --block-aware */
/* @SymbolSize: 220 */
/* Address: 0x801531B8 */
/* Size: 220 bytes (55 insns) */
#include "types.h"

extern s32 taco_glyph_key_tbl[];

extern void taco_pose_keyframe_ease(s32, s32, s32, s32, s32);

void taco_reel_glyph_disp(s32 x, s32 idx, s32 mode)
{
    s32 *p;
    s32 i;
    s32 cx;

    if (mode < 10) {
        i = 0;
        cx = x;
        p = &taco_glyph_key_tbl[idx];
        do {
            taco_pose_keyframe_ease(cx, *p, 14, 0, 0);
            i += 3;
            cx += 3;
            p++;
        } while (i != 21);
        return;
    }
    if (mode < 10) {
        return;
    }
    if (mode < 20) {
        i = 0;
        cx = x;
        p = &taco_glyph_key_tbl[idx];
        do {
            taco_pose_keyframe_ease(cx, *p, 15, 0, 0);
            i += 3;
            cx += 3;
            p++;
        } while (i != 21);
    }
}
