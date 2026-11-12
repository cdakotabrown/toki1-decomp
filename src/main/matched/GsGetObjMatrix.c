/* Function: GsGetObjMatrix */
/* Address: 0x8009F694 */
/* Size: 348 bytes (87 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct { s32 w[8]; } Mtx;

typedef struct {
    u32 flags;
    Mtx mat;
} SndWork;

extern Mtx GsWSMATRIX;
extern Mtx GsLSMATRIX;

extern void MulMatrix(Mtx *dst, Mtx *src);
extern void ApplyMatrixLV(Mtx *m, s32 *in, s32 *out);

void GsGetObjMatrix(SndWork *arg0)
{
    s32 vec[3];

    if (arg0->flags & 0x20000000) {
        GsLSMATRIX = arg0->mat;
        GsLSMATRIX.w[5] = arg0->mat.w[5];
        GsLSMATRIX.w[6] = arg0->mat.w[6];
        GsLSMATRIX.w[7] = arg0->mat.w[7];
    } else {
        GsLSMATRIX = GsWSMATRIX;
        MulMatrix(&GsLSMATRIX, &arg0->mat);
        ApplyMatrixLV(&GsWSMATRIX, &arg0->mat.w[5], vec);
        GsLSMATRIX.w[5] = vec[0] + GsWSMATRIX.w[5];
        GsLSMATRIX.w[6] = vec[1] + GsWSMATRIX.w[6];
        GsLSMATRIX.w[7] = vec[2] + GsWSMATRIX.w[7];
    }
}
