/* Address: 0x80062AEC */
/* Size: 736 bytes (184 instructions) */
/* Region: Game (IDO 5.2) */
/* 4. `@CompileEnv: IDO52_UGEN_TEMP_REG=34:t2,35:t7,36:t8,37:t9,38:t0` -- */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=34:t2,35:t7,36:t8,37:t9,38:t0 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x26c --offset2 0x270 */
/* @PostProc: swap_instructions --offset1 0x280 --offset2 0x284 */
/* @PostProc: patch_insn_reg_field 0x64:rs:t6:v0 0x64:rt:v0:t6 0x280:rs:t7:t8 0x280:rt:t8:t7 */
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
    s32 flag;
    MATRIX mat;
    u8 rest[44];
} Obj;

extern s32 frame_counter;
extern s32 work_fuku[];
extern MATRIX GsIDMATRIX;
extern Obj coord_buf[];

extern s32 rsin();
extern s32 rcos();
extern void RotMatrix();
extern void ScaleMatrix();
extern void MulMatrix();
extern s32 abs();

void title_obj_move()
{
    s32 i;
    s32 k;
    SVECTOR rot;
    MATRIX ma;
    MATRIX mb;
    VECTOR scale;
    s32 r;

    for (i = 0; i < 13; i++) {
        k = i & 3;
        rot.vz = (k + 2) * frame_counter * -2 - 2;
        rot.vx = rsin(frame_counter * 10 * (k + 1)) / 12 + 2045;
        rot.vy = rcos(frame_counter * 10 * (k + 1)) / 12 - 5;
        ma = GsIDMATRIX;
        mb = GsIDMATRIX;
        RotMatrix(&rot, &ma);
        ma.t[0] = 0;
        ma.t[1] = 0;
        ma.t[2] = 0;
        scale.vx = 4096;
        scale.vy = 4096;
        scale.vz = 4096;
        ScaleMatrix(&mb, &scale);
        MulMatrix(&ma, &mb);
        ma.t[0] = work_fuku[i + 1];
        ma.t[1] = -work_fuku[i + 1] / 2 + rsin((frame_counter & 0xFFF) * (k + 10)) / 32;
        ma.t[2] = ((0 - work_fuku[i + 1]) << 2) - 400;
        r = abs(rsin(frame_counter));
        coord_buf[i].mat = ma;
        work_fuku[i + 1] = (r & 1) + work_fuku[i + 1] + 2;
        coord_buf[i].flag = 0;
    }
}
