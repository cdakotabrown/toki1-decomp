/* Function: GsGetObjLight (GsGetObjLight) */
/* Address: 0x8009F600 */
/* Size: 148 bytes (37 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc272 */
/* @CompileFlags: -O1 --fill-epilogue */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0x8 */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x3c */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x5c */
#include "types.h"

typedef struct {
    short m[3][3];
    long t[3];
} MATRIX;

extern MATRIX GsLIGHTWSMATRIX;
extern void PushMatrix(void);
extern void MulMatrix(MATRIX *, MATRIX *);
extern void PopMatrix(void);
extern void SetLightMatrix(MATRIX *);

void GsGetObjLight(u8 *arg0) {
    MATRIX local;
    MATRIX *lp;

    local = GsLIGHTWSMATRIX;
    PushMatrix();
    lp = &local;
    MulMatrix(lp, (MATRIX *)(arg0 + 36));
    PopMatrix();
    SetLightMatrix(lp);
}
