/* @SymbolStart: 0x80155E68 */
/* @SymbolSize: 116 */
/* func_80155E70 - EVENT overlay, Address: 0x80155E70, Size: 112 bytes (28 insns) */
#include "types.h"

extern u8 D_800B1746;

extern s16 D_80094730;
extern u8 D_8012531C;
extern s32 D_800EECBC;

extern void func_80011DFC();

void func_80155E70(void) {
    u8 t6_v = D_800B1746;

    u8 branch;
    s16 timer;

    if (t6_v != 7) {
        goto skip_to_advance;
    }

    branch = D_8012531C;
    if (branch != 0) {
        goto skip_to_advance;
    }

    timer = D_80094730;
    D_80094730 = timer + D_800EECBC;
    func_80011DFC();
    return;

skip_to_advance:

    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
}
