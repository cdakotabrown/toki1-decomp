/* Address: 0x80155370 (cross-prologue head absorbed) .. 0x801553A0 */
/* Size: 208 bytes (52 instructions) */
/* @SymbolStart: 0x80155370 */
/* @SymbolSize: 208 */
#include "types.h"

extern u8 D_800B0860[];
extern u8 D_800B1746;
extern volatile u8 D_800B0896;
extern u8 D_800B0897;
extern s32 D_800B0948;

extern void func_80011E28(s32);
extern void func_80011E8C(s32);
extern void func_80011DFC(void);

void func_801553A0(void) {
    s32 rec;
    s32 other;

    rec = *(s32 *)(D_800B0860 + D_800B1746 * 52 + 436);
    if ((rec & 0xF) == D_800B0896 &&
        ((u32)(rec << 23) >> 27) == D_800B0897) {
        func_80011E28(80);
        func_80011E8C(2);
        return;
    }
    other = D_800B0948;
    if (D_800B0896 == (other & 0xF) &&
        D_800B0897 == ((u32)(other << 23) >> 27)) {
        func_80011E28(80);
        func_80011E8C(3);
        return;
    }
    func_80011DFC();
}
