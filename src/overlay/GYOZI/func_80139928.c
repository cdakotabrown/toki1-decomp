/* @SymbolStart: 0x80139920 */
/* @SymbolSize: 96 */
/* func_80139928 -- GYOZI overlay, Address: 0x80139920, Size: 96 bytes (24 insns) */
#include "types.h"

extern void func_8013B980(void);
extern void func_8013B9FC(void);
extern void func_8013BA84(void);

extern u8 D_801474B8;

void func_80139928(void)
{
    u32 temp = D_801474B8;

    switch (temp) {
    case 0:
        func_8013B980();
        break;
    case 1:
        func_8013B9FC();
        break;
    default:
        func_8013BA84();
        break;
    }
}
