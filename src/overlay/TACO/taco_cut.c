/* @CompileEnv: IDO52_UGEN_TEMP_REG=10:t6,11:t7,12:t8,13:t9,14:t0,15:t1,16:t2 */
/* Address: 0x8014CAF4 */
/* Size: 1756 bytes (439 insns) */
#include "types.h"
#include "tokimeki/libgs_types.h"

typedef struct {
    s16 m[3][3];
    s32 t[3];
} Mtx;

typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad;
} SV;

extern void ApplyMatrixSV(Mtx *, SV *, SV *);
extern void taco_pose_keyframe_ease();
extern void RotMatrixZ(s32, void *);
extern void RotMatrixX(s32, void *);
extern void RotMatrixY(s32, void *);

extern Mtx GsIDMATRIX;
extern s16 taco_angle_tbl[][4];
extern u8 *taco_actor_rec;
extern s32 taco_cut_ang_tbl[5];
extern s32 taco_cut_ang_tbl_5[3];
extern s32 taco_cut_ang_tbl_8[2];
extern s32 taco_cut_ang_tbl_10;
extern s32 taco_cut_ang_tbl_11[2];
extern LIBGS_COORD2 taco_coord[];

void taco_cut(s32 tick)
{
    Mtx m;
    SV in;
    SV out;
    s32 i;

    if (tick < 10) {
        for (i = 0; i < 5; i++) {
            taco_pose_keyframe_ease(i + 1, taco_cut_ang_tbl[i], 1, 0, 0);
        }

        for (i = 0; i < 3; i++) {
            taco_pose_keyframe_ease(i + 25, taco_cut_ang_tbl_5[i], 1, 0, 0);
        }

        for (i = 0; i < 2; i++) {
            taco_pose_keyframe_ease(i + 29, taco_cut_ang_tbl_8[i], 1, 0, 0);
        }

        taco_pose_keyframe_ease(8, taco_cut_ang_tbl_10, 1, 0, 0);

        for (i = 0; i != 2; i++) {
            taco_pose_keyframe_ease(i + 21, taco_cut_ang_tbl_11[i], 1, 0, 0);
        }
    } else if (tick >= 14 && tick < 24) {
        for (i = 0; i < 5; i++) {
            taco_pose_keyframe_ease(i + 1, taco_cut_ang_tbl[i], 2, 0, 0);
        }

        for (i = 0; i < 3; i++) {
            taco_pose_keyframe_ease(i + 25, taco_cut_ang_tbl_5[i], 2, 0, 0);
        }

        for (i = 0; i != 2; i++) {
            taco_pose_keyframe_ease(i + 29, taco_cut_ang_tbl_8[i], 2, 0, 0);
        }

        taco_pose_keyframe_ease(8, taco_cut_ang_tbl_10, 2, 0, 0);
    } else if (tick >= 30 && tick < 40) {
        taco_pose_keyframe_ease(26, taco_cut_ang_tbl_5[1], 3, 0, 0);
        taco_pose_keyframe_ease(8, taco_cut_ang_tbl_10, 3, 0, 0);
    } else if (tick >= 44 && tick < 54) {
        taco_pose_keyframe_ease(26, taco_cut_ang_tbl_5[1], 4, 0, 0);
        taco_pose_keyframe_ease(8, taco_cut_ang_tbl_10, 4, 0, 0);
    } else if (tick >= 54 && tick < 64) {
        for (i = 0; i < 5; i++) {
            taco_pose_keyframe_ease(i + 1, taco_cut_ang_tbl[i], 3, 0, 0);
        }

        taco_pose_keyframe_ease(25, taco_cut_ang_tbl_5[0], 3, 0, 0);
        taco_pose_keyframe_ease(26, taco_cut_ang_tbl_5[1], 5, 0, 0);
        taco_pose_keyframe_ease(27, taco_cut_ang_tbl_5[2], 3, 0, 0);

        for (i = 0; i != 2; i++) {
            taco_pose_keyframe_ease(i + 29, taco_cut_ang_tbl_8[i], 3, 0, 0);
        }

        taco_pose_keyframe_ease(8, taco_cut_ang_tbl_10, 5, 0, 0);
    } else if (tick >= 64 && tick < 74) {
        for (i = 0; i < 5; i++) {
            taco_pose_keyframe_ease(i + 1, taco_cut_ang_tbl[i], 4, 0, 0);
        }

        taco_pose_keyframe_ease(25, taco_cut_ang_tbl_5[0], 4, 0, 0);
        taco_pose_keyframe_ease(26, taco_cut_ang_tbl_5[1], 6, 0, 0);
        taco_pose_keyframe_ease(27, taco_cut_ang_tbl_5[2], 4, 0, 0);

        for (i = 0; i < 2; i++) {
            taco_pose_keyframe_ease(i + 29, taco_cut_ang_tbl_8[i], 4, 0, 0);
        }

        taco_pose_keyframe_ease(8, taco_cut_ang_tbl_10, 6, 0, 0);

        for (i = 0; i != 2; i++) {
            taco_pose_keyframe_ease(i + 21, taco_cut_ang_tbl_11[i], 2, 0, 0);
        }
    }

    if (tick >= 40 && tick < 45) {
        m = GsIDMATRIX;

        RotMatrixX(((s32)taco_angle_tbl[0][0] << 12) / 360, &m);
        RotMatrixZ(((s32)taco_angle_tbl[0][2] << 12) / 360, &m);
        RotMatrixY(((s32)taco_angle_tbl[0][1] << 12) / 360, &m);

        in.vx = (s16)taco_coord[8].coord.t[0];
        in.vy = (s16)taco_coord[8].coord.t[1];
        in.vz = (s16)taco_coord[8].coord.t[2];
        ApplyMatrixSV(&m, &in, &out);

        *(u8 *)(taco_actor_rec + 2721) = 64;
        *(s16 *)(taco_actor_rec + 2836) = (u32)taco_coord[0].coord.t[0] + out.vx;
        *(s16 *)(taco_actor_rec + 2838) = (u32)taco_coord[0].coord.t[1] + out.vy;
        *(s16 *)(taco_actor_rec + 2840) = 2048;
        *(u8 *)(taco_actor_rec + 2852) = 40;
    } else if (tick == 45) {
        *(u8 *)(taco_actor_rec + 2721) = 128;
    }
}
