/* Address: 0x8015349C (cross-prologue head absorbed) .. 0x801534B4 */
/* Size: 156 bytes (39 instructions) */
/* @SymbolStart: 0x8015349C */
/* @SymbolSize: 156 */
#include "types.h"

extern s32 func_8002336C(s32 id);
extern void func_8004A8EC(s32 arg);
extern void func_80033E88(void);
extern void func_80011DFC(void);

extern s32 D_800EECBC;
extern s32 D_800EECC8;
extern u8 D_800B0896;
extern u8 D_80125310;
extern s16 D_8012437C;
extern s16 D_80124380;
extern u8 D_800B1746;
extern u8 D_800B1AF6;

void func_801534B4(void)
{
    D_800EECC8 = D_800EECBC;
    if (D_800B0896 == 8) {
        s32 r;

        D_80125310 = 1;
        D_8012437C = 0;
        D_80124380 = 0;
        r = func_8002336C(D_800B1746);
        func_8004A8EC(r);
        func_80033E88();
        func_80011DFC();
    } else {
        func_80011DFC();
        D_800B1AF6 = D_800B1AF6 + 16;
    }
}
