/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all IDO52_AS1_KEEP_ANDI=all IDO52_AS1_PASS1_VOLATILE_OK=all IDO52_UGEN_TEMP_REG=2:t7,3:t8,4:t9,5:t0 */
/* Address: 0x80153540 (true entry 0x80153538 -- cross-prologue head absorbed */
/* @SymbolStart: 0x80153538 */
/* @SymbolSize: 212 */
#include "types.h"

extern volatile u8 D_800B1AF6;
extern s32 D_800EECC4;

extern s32 func_80046600(void);
extern s32 func_800464E8(void);
extern s32 func_80011DFC(void);

s32 func_80153540(void) {
    s32 after;
    s32 before;

    switch ((u32)D_800EECC4) {
    case 0:
        after = D_800B1AF6;
        before = after;
        func_80046600();
        after = D_800B1AF6;
        if (before != after) {
            D_800B1AF6 = after = (after - 1) & 0xFF;
            D_800EECC4 = D_800EECC4 + 1;
        }
        break;
    case 1:
        before = D_800B1AF6;
        func_800464E8();
        after = D_800B1AF6;
        if (before != after) {
            D_800B1AF6 = after = (after - 1) & 0xFF;
            D_800EECC4 = D_800EECC4 + 1;
        }
        break;
    default:
        func_80011DFC();
        break;
    }
}
