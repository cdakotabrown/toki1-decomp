/* @PostProc: patch_stack_complete --old 120 --new 128 */
/* @PostProc: remap_stack_slots --map '36:40,40:44,44:48,48:56,50:58,52:60,56:64,60:68,64:72,68:76,72:80,76:84,80:88,84:92,88:96,92:100,96:104,100:108,104:112,108:116,112:120,116:124' */
/* @PostProc: patch_insn_reg_field 0x148:rt:a0:v1 0x14c:rt:t7:t8 0x150:rs:a0:v1 0x150:rt:a0:v1 0x154:rs:t7:t8 0x154:rt:t7:t8 0x158:rt:t8:t7 0x15c:rt:t8:t7 0x160:rt:a0:v1 0x164:rt:t7:t8 0x1d0:rt:a0:v1 0x1dc:rs:a0:v1 */
#include "types.h"

typedef struct { s32 w[8]; } Rec32;

extern void RotMatrix(void *a0, void *a1);
extern void MulMatrix(void *a0, void *a1);
extern void ScaleMatrix(void *a0, void *a1);

extern Rec32 GsIDMATRIX;
extern Rec32 coord_buf_0_coord;
extern s32   coord_buf;
extern s16 taiiku_tama_ang[];
extern s32 taiiku_tama_scale;
extern s16 taiiku_tama_sy;
extern s16 taiiku_tama_sx;

void taiiku_mtx_set(void)
{
    Rec32 buf96;
    Rec32 buf64;
    s16 hdr[4];
    s32 col[3];
    s32 v1;
    s32 t8;
    s32 v0;

    buf96 = GsIDMATRIX;
    hdr[0] = 0;
    hdr[2] = 0;
    hdr[1] = taiiku_tama_ang[1];
    RotMatrix(hdr, &buf96);

    buf64 = GsIDMATRIX;
    hdr[1] = 0;
    hdr[2] = 0;
    hdr[0] = taiiku_tama_ang[0];
    RotMatrix(hdr, &buf64);

    MulMatrix(&buf96, &buf64);
    v0 = taiiku_tama_scale;
    col[0] = v0;
    col[1] = v0;
    col[2] = v0;
    ScaleMatrix(&buf64, col);

    MulMatrix(&buf96, &buf64);
    v1 = taiiku_tama_sy;
    t8 = taiiku_tama_sx;
    buf96.w[7] = 7000;
    buf96.w[6] = v1;
    buf96.w[5] = t8;

    v0 = taiiku_tama_scale;
    coord_buf_0_coord = buf96;
    coord_buf = 0;
    taiiku_tama_scale = v0 - 128;

    if ((v1 >> 3) >= -63) {
        taiiku_tama_sy = v1 - 8;
    }
}
