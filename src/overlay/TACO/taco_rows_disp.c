/* @PostProc: scheduler_normalizer */
/* Address: 0x801521BC */
/* Size: 196 bytes (49 insns) */
#include "types.h"

extern u8 taco_panel_key_tbl[];
extern u8 taco_panel_mode_tbl[];
extern u8 taco_glyph_key_tbl[];

extern void node_pose_set(s32 a0, s32 a1, s32 a2, s32 a3, s32 s16, s32 s20);
extern void angle_set(s32 a0, s32 a1);

void taco_rows_disp(s32 id, s32 pal)
{
    s32 end;
    s32 i;
    s32 id2;
    u8 *wp;
    u8 *bp;
    u8 *cp;

    end = id + 19;
    if (id >= end) {
        return;
    }

    i = id;
    id2 = id * 2;
    wp = taco_panel_key_tbl + id2 * 4;
    bp = taco_panel_mode_tbl + id2;
    cp = taco_glyph_key_tbl + pal * 4;
    do {
        node_pose_set(i, 0,
                      *(s32 *)(wp - 8), *(s32 *)(wp - 4),
                      bp[-2], bp[-1]);
        angle_set(i, *(s32 *)cp);
        i += 3;
        wp += 24;
        bp += 6;
        cp += 4;
    } while (i < end);
}
