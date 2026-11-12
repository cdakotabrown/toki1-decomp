/* @SymbolStart: 0x80156144 */
/* @SymbolSize: 160 */
/* Address: 0x8015614C */
/* Size: 152 bytes (38 instructions) */
#include "types.h"

extern void func_8004CF30(void);
extern void func_80011DFC(void);

extern u8 D_8012531C;
extern s32 D_800EECBC;
extern volatile u8 D_800B1746;

void func_8015614C(void)
{
    if (D_800B1746 == 5 && D_8012531C == 0 && D_800EECBC == 2) {
        func_8004CF30();
    }

    if (D_800B1746 == 5 && D_8012531C == 1 && D_800EECBC == 0) {
        func_8004CF30();
    } else {
        func_80011DFC();
    }
}
