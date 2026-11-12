/* @SymbolStart: 0x8015B2BC */
/* @SymbolSize: 260 */
/* @PostProc: patch_stack_complete --old 48 --new 88 */
/* @PostProc: remap_stack_slots --from 28 --to 40 --from 32 --to 44 --from 36 --to 48 */
#include "types.h"

extern void RotMatrixZ(s32, void *);
extern void RotMatrixX(s32, void *);
extern void RotMatrixY(s32, void *);

extern u8 taco_coord[];
extern u8 taco_angle_tbl[];

void taco_rec_pose_set(s32 a0, s16 *a1)
{
    u8 *v0;
    s16 *v1;

    v0 = taco_coord + a0 * 80;
    *(s32 *)(v0 + 24) += a1[0];
    *(s32 *)(v0 + 28) += a1[1];
    *(s32 *)(v0 + 32) += a1[2];

    v1 = (s16 *)(taco_angle_tbl + a0 * 8);
    v1[0] = a1[3];
    v1[2] = a1[4];
    v1[1] = a1[5];

    RotMatrixX(((s32)v1[0] << 12) / 360, v0 + 4);
    RotMatrixZ(((s32)v1[2] << 12) / 360, v0 + 4);
    RotMatrixY(((s32)v1[1] << 12) / 360, v0 + 4);
    *(s32 *)v0 = 0;
}
