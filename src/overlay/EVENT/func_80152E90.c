/* @SymbolStart: 0x80152E80 */
/* @SymbolSize: 60 */
/* Address: 0x80152E80 (absorbed cross-prologue head) .. 0x80152E90 */
/* Size: 60 bytes (15 instructions) */
#include "types.h"

extern s16 D_80094730;
extern u8 D_8012531C;

extern void func_80011DFC(void);

void func_80152E90(void) {
    D_80094730 = D_80094730 + (D_8012531C << 1);
    func_80011DFC();
}
