/* Address: 0x8013D8D4 (true start 0x8013D8A4: the `li t6,3` plus the first */
/* Size: 0x8013D8FC - 0x8013D8A4 = 88 bytes (22 instructions) */
/* @SymbolStart: 0x8013D8A4 */
/* @SymbolSize: 88 */
#include "types.h"

extern u8 D_80125320;
extern s32 D_80120334;
extern s32 D_80120348;
extern s32 D_8012035C;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;

extern void func_80011DFC(void);

void func_8013D8D4(void) {
    D_80125320 = 3;
    D_80120678 = D_80120334;
    D_8012067C = D_80120348;
    D_80120680 = D_8012035C;
    func_80011DFC();
}
