/* @SymbolStart: 0x80155074 */
/* @SymbolSize: 124 */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
/* Address: 0x80155074 (absorbed cross-prologue head) .. 0x80155084 */
/* Size: 124 bytes (31 instructions) */
#include "types.h"

extern u8 D_800B0F31;
extern u8 D_800B0F32;
extern u8 D_800B0F33;
extern u8 D_800B0F34;
extern u8 D_800B0895;

extern void func_801190F0(void);
extern void func_80011DFC(void);

void func_80155084(void) {
    if (D_800B0F32 == D_800B0F31 && D_800B0F33 == D_800B0F31 &&
        D_800B0F34 == D_800B0F31 && (u32)D_800B0895 >= 96U) {
        func_801190F0();
    } else {
        func_80011DFC();
    }
}
