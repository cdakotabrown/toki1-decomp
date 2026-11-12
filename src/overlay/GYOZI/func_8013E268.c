/* Address: 0x8013E268, Size: 200 bytes (50 instructions) */
/* @SymbolStart: 0x8013E244 */
/* @SymbolSize: 236 */
/* @PostProc: patch_insn_reg_field 0x44:rt:a0:v1 0x48:rs:a0:v1 */
#include "types.h"

extern void func_8004DE1C(void);

extern u8 D_800F53DE;
extern s16 D_800F54A6;
extern s16 D_800F54AE;
extern u8 D_800F594F;

void func_8013E268(void)
{
    u8 count;

    if ((u32)D_800F54A6 >= (u32)(D_800F53DE * 25 - 2225)) {
        count = D_800F594F;
        if (count != 0) {
            D_800F594F = count + 1;
            func_8004DE1C();
            func_8004DE1C();
            func_8004DE1C();
            func_8004DE1C();
            return;
        }
    }

    if ((u32)D_800F54AE >= (u32)(D_800F53DE * 30 -
                                 (D_800F53DE == 0x61) * 10 - 2780)) {
        func_8004DE1C();
    } else {
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
    }
}
