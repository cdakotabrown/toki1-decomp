/* @SymbolStart: 0x8013D4FC */
/* @SymbolSize: 1180 */
/* Address: 0x8013D4FC (symbol named for its old prologue address 0x8013D50C) */
/* Size: 1180 bytes (295 insns) */
#include "types.h"

typedef struct {
    s16 m[3][3];
    s32 t[3];
} MATRIX;

typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad;
} SVECTOR;

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
    s32 pad;
} VECTOR;

extern s16 para_un;
extern s32 taiiku_tama_girl;
extern s32 taiiku_tama_pstep;
extern s32 taiiku_tama_gstep;
typedef struct {
    s16 yaw;
    s16 pitch;
} YAWPITCH;
YAWPITCH taiiku_tama_ang = {0, 0};
extern s32 taiiku_tama_vel[];
extern s16 taiiku_tama_sx;
extern s16 taiiku_tama_sy;
extern s32 taiiku_tama_fall;
extern MATRIX GsIDMATRIX;
extern s32 coord_buf;
extern MATRIX coord_buf_0_coord;
extern s32 taiiku_tama_ppush[];
extern s32 taiiku_tama_gpush[];

extern s32 abs(s32);
extern void RotMatrix(SVECTOR *, MATRIX *);
extern void MulMatrix(MATRIX *, MATRIX *);
extern void ScaleMatrix(MATRIX *, VECTOR *);

void taiiku_camera_mtx_set(void)
{
    s32 x;
    s32 result;
    SVECTOR sv;
    MATRIX m1;
    MATRIX m2;
    VECTOR sc;
    s16 v;
    s32 a;

    v = para_un;
    result = abs(taiiku_tama_vel[1] * 8);
    if (taiiku_tama_ppush[1] > 0 && taiiku_tama_gpush[1] > 0) {
        if (v >= 160) {
            result = abs(taiiku_tama_vel[1] * 16);
        } else if (v >= 100) {
            result += abs(taiiku_tama_vel[0] * 2) * 3;
        } else if (v >= 75) {
            result += abs(taiiku_tama_vel[0] * 4);
        } else if (v >= 50) {
            result += abs(taiiku_tama_vel[0] * 2);
        }
    } else if (taiiku_tama_girl == 2) {
        if (v >= 400) {
            result = abs(taiiku_tama_vel[1] * 16);
        } else if (v >= 300) {
            result += abs(taiiku_tama_vel[0] * 2) * 3;
        } else if (v >= 200) {
            result += abs(taiiku_tama_vel[0] * 4);
        } else if (v >= 100) {
            result += abs(taiiku_tama_vel[0] * 2);
        }
    }

    if (taiiku_tama_fall == 0) {
        if (taiiku_tama_girl == 6) {
            ((s16 *)&result)[1] += 8;
        }
        if (taiiku_tama_girl == 10) {
            ((s16 *)&result)[1] += 4;
        }
    }

    if (taiiku_tama_fall == 0) {
        taiiku_tama_ang.yaw = taiiku_tama_ang.yaw - ((s16 *)&result)[1];
    } else if (taiiku_tama_pstep == 4 && taiiku_tama_gstep == 4) {
        if (taiiku_tama_fall == 1) {
            taiiku_tama_ang.yaw = taiiku_tama_ang.yaw + 8;
            taiiku_tama_ang.pitch = 1024;
        } else {
            taiiku_tama_ang.yaw = taiiku_tama_ang.yaw + 8;
            taiiku_tama_ang.pitch = -1024;
        }
    }

    m1 = GsIDMATRIX;
    sv.vx = 0;
    sv.vy = taiiku_tama_ang.pitch;
    sv.vz = 0;
    RotMatrix(&sv, &m1);

    m2 = GsIDMATRIX;
    sv.vx = taiiku_tama_ang.yaw;
    sv.vy = 0;
    sv.vz = 0;
    RotMatrix(&sv, &m2);

    MulMatrix(&m1, &m2);
    sc.vx = 4608;
    sc.vy = 4608;
    sc.vz = 4608;
    ScaleMatrix(&m2, &sc);
    MulMatrix(&m1, &m2);

    m1.t[0] = taiiku_tama_sx;
    m1.t[1] = taiiku_tama_sy;
    m1.t[2] = 7000;
    coord_buf_0_coord = m1;
    coord_buf = 0;
}
