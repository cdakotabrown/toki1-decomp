/* @SymbolStart: 0x801544DC */
/* @SymbolSize: 196 */
/* Address: 0x801544E4 */
/* Size: 188 bytes (47 instructions) */
#include "types.h"

extern s32 D_800B1AF0;
extern s16 D_80124378;
extern u8 D_80125318;
extern s32 D_80124374;
extern s32 D_801241D0;
extern s32 D_801241E0;
extern s32 D_801241F0;
extern s32 D_8009471C;
extern s32 D_80094720;
extern s32 D_80094724;
extern s32 D_80094728;
extern s32 D_8009472C;
extern s32 D_800EECBC;

extern s32 func_80049B20(s32, s32, s32);
extern void func_80011DFC(void);

void func_801544E4(void) {
    u32 once;
    s32 first;

    once = D_800B1AF0;
    first = (once < 1);
    D_800B1AF0 = once + 1;
    if (first) {
        D_80124378 = D_80124378 + D_80125318;
    }
    D_8009471C = (s32)&D_80124374;
    D_80094720 = (s32)&D_80124378;
    D_80094724 = D_801241D0;
    D_80094728 = D_801241E0;
    D_8009472C = D_801241F0;
    if (func_80049B20(D_800EECBC, 0, 0) != 0) {
        func_80011DFC();
    }
}
