/* Function: func_80064510 */
/* Address: 0x80064510 */
/* Size: 188 bytes (47 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Status: BLOCKED - ASM-only, cannot be compiled from C */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 D_80127F3B;
extern s16 D_80127F6A;
extern s16 D_80127F26;
extern u8 D_80127FC3;
extern s16 D_80127FAE;
extern u8 D_80127F7F;

extern s16 para_kon;
extern s16 D_80127FDC;
extern s16 D_80127FEA;
extern s16 D_80127FEE;

extern s32 keta_n(s32, s32);

void func_80064510(void) {
    s16 s0_val;
    s32 result;

    s0_val = para_kon;
    result = keta_n((s16)s0_val, 3);
    D_80127FDC = (s16)(result + 68);
    D_80127FEA = 15;
    D_80127FEE = -60;
}
