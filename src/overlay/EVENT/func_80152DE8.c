/* Address: 0x80152DC0 (cross-prologue head absorbed) .. 0x80152DE8 */
/* Size: 96 bytes (24 instructions) */
/* @SymbolStart: 0x80152DC0 */
/* @SymbolSize: 96 */
#include "types.h"

extern u8 D_800B0860[];
extern u8 D_800B1746;
extern s16 D_8012437C;

extern void func_80011DFC(void);

void func_80152DE8(void) {
    if (D_800B0860[D_800B1746 * 52 + 465] == 0) {
        D_8012437C = D_8012437C + 1;
    }
    func_80011DFC();
}
