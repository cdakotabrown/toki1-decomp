/* @SymbolStart: 0x80155C40 */
/* @SymbolSize: 92 */
/* Address: 0x80155C48 */
#include "types.h"

extern u8 D_800B1746;

extern s32 D_800EECBC;
extern u8 D_800B0B53;

extern void func_80011DFC(void);

void func_80155C48(void) {
    u8 t6_in_v = D_800B1746;

    if (t6_in_v == 6) {
        if (D_800B0B53 != 0) {
            if (D_800EECBC == 2) {
                D_800EECBC = 3;
            }
        }
    }
    func_80011DFC();
}
