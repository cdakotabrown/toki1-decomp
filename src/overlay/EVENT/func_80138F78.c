/* Address: 0x80138F78 (true start 0x80138F40: the head stores are hoisted */
/* Size: 0x80138FA4 - 0x80138F40 = 100 bytes (25 instructions) */
/* @SymbolStart: 0x80138F40 */
/* @SymbolSize: 100 */
/* @PostProc: scheduler_normalizer */
#include "types.h"

extern u8 D_800EB02F;
extern u8 D_800EB02B;
extern s16 D_800EB030;
extern s16 D_800EB03E;
extern s16 D_800EB040;
extern u8 D_800EB02A;

extern void func_80011DFC(void);

void func_80138F78(void) {
    D_800EB02F = 0x80;
    D_800EB02B |= 0x80;
    D_800EB030 = 0;
    D_800EB03E = 4;
    D_800EB040 = 0;
    D_800EB02A = 1;
    func_80011DFC();
}
