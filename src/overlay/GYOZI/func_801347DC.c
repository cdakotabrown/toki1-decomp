/* @SymbolStart: 0x801347D4 */
/* @SymbolSize: 104 */
/* func_801347DC -- GYOZI overlay, Address: 0x801347D4, Size: 104 bytes (26 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern u8 D_80146178;
extern s16 D_80145EB4;

void func_801347DC(void)
{
    u32 temp = D_80146178;

    if (temp == 0) {
        D_80145EB4 += 2;
    } else if (temp != 3 && temp != 4) {
        D_80145EB4 += 1;
    }
    func_8004DE1C();
}
