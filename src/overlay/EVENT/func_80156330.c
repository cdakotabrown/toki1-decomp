/* @SymbolStart: 0x80156328 */
/* @SymbolSize: 180 */
/* Address: 0x80156330 */
/* Size: 172 bytes (43 instructions) */
#include "types.h"

extern void func_8004CF30(void);
extern void func_80011DFC(void);

extern s32 D_800EECBC;
extern u8 D_8012531C;
extern volatile u8 D_800B1746;
extern s16 D_800B0956;

void func_80156330(void)
{
    if (D_800B1746 == 10 && D_800EECBC == 0 && D_8012531C == 0) {
        func_8004CF30();
    }

    if (D_800B1746 == 10 && D_800EECBC == 0 && D_8012531C == 1 &&
        D_800B0956 < 120) {
        func_8004CF30();
    } else {
        func_80011DFC();
    }
}
