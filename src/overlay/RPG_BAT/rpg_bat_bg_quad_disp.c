/* @PostProc: patch_stack_complete --old 128 --new 136 */
/* @PostProc: remap_stack_slots --from 148 --to 156 --from 144 --to 152 --from 128 --to 136 --from 124 --to 132 --from 120 --to 128 --from 116 --to 124 --from 112 --to 120 --from 108 --to 116 --from 104 --to 112 --from 100 --to 108 --from 96 --to 104 --from 92 --to 100 --from 88 --to 96 --from 84 --to 92 --from 80 --to 88 --from 76 --to 84 --from 72 --to 80 --from 68 --to 76 --from 64 --to 72 --from 60 --to 68 --from 56 --to 64 */
/* @PostProc: scheduler_normalizer */
#include "types.h"
#include "tokimeki/class_w.h"
extern CLASS_W class_w[];

extern void rpg_bat_quad_disp(void *p, void *q, s32 half, s32 arg6, s32 iter);

void rpg_bat_bg_quad_disp(s32 x, s32 y, s32 c0, s32 c1, s32 arg5, s32 arg6)
{
    s32 P[8];
    s32 Q[8];
    s32 x319, y319;

    P[0] = x;       P[1] = c0; P[2] = x + 127; P[3] = c0;
    P[4] = y;       P[5] = c1; P[6] = y + 127; P[7] = c1;
    Q[0] = 0;       Q[1] = c0; Q[2] = 127;     Q[3] = c0;
    Q[4] = 0;       Q[5] = c1; Q[6] = 127;     Q[7] = c1;
    rpg_bat_quad_disp(P, Q, ((u32)(class_w[127].bright + arg5)) >> 1, arg6, 1);

    P[0] = x + 128; P[1] = c0; P[2] = x + 255; P[3] = c0;
    P[4] = y + 128; P[5] = c1; P[6] = y + 255; P[7] = c1;
    Q[0] = 0;       Q[1] = c0; Q[2] = 127;     Q[3] = c0;
    Q[4] = 0;       Q[5] = c1; Q[6] = 127;     Q[7] = c1;
    rpg_bat_quad_disp(P, Q, ((u32)(class_w[127].bright + arg5)) >> 1, arg6, 2);

    x319 = x + 319;
    y319 = y + 319;
    P[0] = x + 256; P[1] = c0; P[2] = x319; P[3] = c0;
    P[4] = y + 256; P[5] = c1; P[6] = y319; P[7] = c1;
    Q[0] = 0;       Q[1] = c0; Q[2] = 63;   Q[3] = c0;
    Q[4] = 0;       Q[5] = c1; Q[6] = 63;   Q[7] = c1;
    rpg_bat_quad_disp(P, Q, ((u32)(class_w[127].bright + arg5)) >> 1, arg6, 3);

    P[0] = x;       P[1] = c0; P[2] = x - 127; P[3] = c0;
    P[4] = y;       P[5] = c1; P[6] = y - 127; P[7] = c1;
    Q[0] = 0;       Q[1] = c0; Q[2] = 127;     Q[3] = c0;
    Q[4] = 0;       Q[5] = c1; Q[6] = 127;     Q[7] = c1;
    rpg_bat_quad_disp(P, Q, ((u32)(class_w[127].bright + arg5)) >> 1, arg6, 1);

    P[0] = x + 383; P[1] = c0; P[2] = x319; P[3] = c0;
    P[4] = y + 383; P[5] = c1; P[6] = y319; P[7] = c1;
    Q[0] = 0;       Q[1] = c0; Q[2] = 63;   Q[3] = c0;
    Q[4] = 0;       Q[5] = c1; Q[6] = 63;   Q[7] = c1;
    rpg_bat_quad_disp(P, Q, ((u32)(class_w[127].bright + arg5)) >> 1, arg6, 3);
}
