/* @SymbolStart: 0x80156F20 */
/* @SymbolSize: 156 */
/* Address: 0x80156F20 (absorbed cross-prologue head) .. 0x80156F30 */
/* Size: 156 bytes (39 instructions) */
#include "types.h"

extern u8  D_801243B0;
extern u8  D_800B0F2D;
extern u8  D_800B1AF6;
extern s16 D_80124384;
extern u8  D_800B0860[];
extern u8  D_800B1746;

extern void func_80011DFC(void);
extern void func_8004C250(void);

void func_80156F30(void) {
    s32 n;

    if (D_800B0F2D != D_801243B0) {
        D_80124384 = 7;
        D_800B1AF6 = (u8)(D_800B1AF6 + 20);
    } else {
        func_80011DFC();
        func_80011DFC();
        n = D_800B1746 * 52;
        *(s16 *)&D_800B0860[n + 430] =
            (s16)(*(s16 *)&D_800B0860[n + 430] + 8);
        func_8004C250();
    }
}
