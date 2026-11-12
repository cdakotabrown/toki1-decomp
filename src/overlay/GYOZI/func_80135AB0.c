/* @SymbolStart: 0x80135AA8 */
/* @SymbolSize: 112 */
/* func_80135AB0 -- GYOZI overlay, Address: 0x80135AA8, Size: 112 bytes (28 insns) */
#include "types.h"

extern void func_8008FB00(void);
extern void func_80050D60(s32 a0, s32 a1);
extern void func_80081D30(s32 a0);

extern u8 D_80145F60;
extern u8 D_800F62CF;

extern s8 D_800F53A0[];

void func_80135AB0(void)
{
    u32 temp = D_80145F60;
    s8 *elem;

    if (temp == 0) {
        elem = D_800F53A0 + D_800F62CF * 56;
        *(s16 *)(elem + 434) = *(s16 *)(elem + 434) + 1;
    }

    func_8008FB00();
    func_80050D60(0, 0);
    func_80081D30(1);
}
