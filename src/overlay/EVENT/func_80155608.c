/* @SymbolStart: 0x80155600 */
/* @SymbolSize: 136 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=1 */
#include "types.h"

extern u8 D_800B1746;

extern u8 D_800B0896;
extern s32 D_800EECBC;

extern void func_80011DFC(void);

s32 func_80155608(void) {
    u8 t6_v = D_800B1746;

    u32 charId;
    s32 one1;
    s32 one2;

    one1 = 1;

    if (t6_v != one1) {
        goto skip_to_advance;
    }

    charId = (u32)D_800B0896;
    if (charId < 6) {
        goto skip_to_advance;
    }
    if (charId >= 9) {
        goto skip_to_advance;
    }

    one2 = 1;

    if (D_800EECBC != one2) {
        goto skip_to_advance;
    }

    func_80011DFC();
    return 0;

skip_to_advance:

    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
}
