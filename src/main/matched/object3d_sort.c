/* Address: 0x8005970C */
/* Size: 764 bytes (191 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=0:v1 IDO52_UGEN_TEMP_REG=9:t3,13:t4,21:t4,32:t1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    s16 m[3][3];
    s32 t[3];
} MATRIX;

typedef struct {
    s16 vx, vy, vz, pad;
} SVECTOR;

typedef struct {
    s32 vx, vy, vz, pad;
} VECTOR;

typedef struct {
    u8 active;
    u8 flags;
    u8 pad2[2];
    s32 f4[1];
    s32 model;
    u8 padC[8];
    s32 f20;
    MATRIX mat;
    u8 pad38[44];
    SVECTOR rot;
    s16 scale[3];
    s16 pad114;
    s16 pos[3];
    s16 pad122;
    s16 vel[3];
    s16 pad130[3];
} Obj;

typedef struct {
    u8 b[68];
} Slot;

extern s32 object3d_top;
extern MATRIX GsIDMATRIX;
extern Slot class_w[];
extern s32 activeBuff;
extern u8 ot_work[][20];

void object3d_sort()
{
    s32 q[2];
    s32 i;
    SVECTOR rot;
    VECTOR scale;
    MATRIX m1;
    MATRIX m2;
    s32 buf132[8];
    s32 buf100[8];
    Obj *p;
    s32 pad[4];

    for (i = object3d_top; i < 160; i += 2) {
        p = (Obj *)&class_w[i];
        if (p->active != 0 && (p->flags & 0x80)) {
            rot.vz = p->rot.vz;
            rot.vx = p->rot.vx;
            rot.vy = p->rot.vy;
            m1 = GsIDMATRIX;
            m2 = GsIDMATRIX;
            RotMatrix(&rot, &m1);
            if (p->scale[0] != 0) {
                scale.vx = p->scale[0];
            } else {
                scale.vx = 4096;
            }
            if (p->scale[1] != 0) {
                scale.vy = p->scale[1];
            } else {
                scale.vy = 4096;
            }
            if (p->scale[2] != 0) {
                scale.vz = p->scale[2];
            } else {
                scale.vz = 4096;
            }
            ScaleMatrix(&m2, &scale);
            MulMatrix(&m1, &m2);
            if (p->flags & 0x10) {
                p->pos[0] += p->vel[0];
                p->pos[1] += p->vel[1];
                p->pos[2] += p->vel[2];
            }
            m1.t[0] = p->pos[0];
            m1.t[1] = p->pos[1];
            m1.t[2] = p->pos[2];
            p->mat = m1;
            p->f20 = 0;
            GsGetLws(p->model, buf100, buf132);
            GsSetLightMatrix(buf100);
            GsGetLs(p->model, buf132);
            GsSetLsMatrix(buf132);
            GsSortObject4(p->f4, ot_work[activeBuff], 6, 0x1F800000);
        }
    }
}
