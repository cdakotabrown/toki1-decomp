/* @SymbolStart: 0x80155824 */
/* @SymbolSize: 196 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=3 */
/* Address: 0x8015582C  Size: 188 bytes (47 instructions) */
#include "types.h"

extern u8 D_800B1746;

extern u8 D_80125320;
extern u8 D_800B0BCF;
extern s16 D_80094734;
extern s16 D_80094730;

extern void func_8004CF30();
extern void func_800201FC();
extern void func_80011DFC();

void func_8015582C(void) {
    u8 t6_v = D_800B1746;

    if (t6_v == 8 && D_80125320 == 3 && D_800B0BCF != 0) {
        D_80094734 = 2;
    }
    func_8004CF30();
    if (D_800B1746 == 8 && D_80125320 == 3 && D_800B0BCF == 0 &&
        D_80094734 == 3) {
        func_800201FC(1);
        D_80094730 = D_80094730 + 1;
        D_80094734 = 0;
        func_80011DFC();
    }
}
