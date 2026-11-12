/* @SymbolStart: 0x8013E330 */
/* @SymbolSize: 236 */
/* @PostProc: patch_insn_reg_field 0x54:rt:t1:t0 0x58:rd:t2:t1 0x58:rt:t1:t0 0x5c:rd:t2:t1 0x5c:rs:t2:t1 0x5c:rt:t1:t0 0x60:rs:t2:t1 0x60:rt:t3:t2 0x64:rt:t3:t2 0x6c:rt:t4:t3 0x74:rt:t4:t3 0x80:rt:t5:t4 0x84:rs:t5:t4 0x84:rt:t5:t4 0x88:rt:t8:t7 0x8c:rs:t8:t7 0x8c:rt:t8:t7 0x94:rs:t5:t4 0x94:rt:t7:t5 0x98:rt:t7:t5 0xa0:rs:t8:t7 0xa0:rt:t6:t8 0xa8:rt:t6:t8 */
/* func_8013E354 -- GYOZI overlay, Address: 0x8013E330 (prologue 0x8013E354), */
/* Size: 236 bytes (59 insns). */
#include "types.h"

extern void func_8004DE1C(void);
extern void func_8008FB00(void);

extern s16 D_800F54A6;
extern u8 D_800F53DE;
extern s16 D_800F54B2;
extern u8 D_800F594F;
extern s16 D_80148620;

void func_8013E354(void)
{
    s32 v0;

    if ((u32)D_800F54A6 >= (u32)(D_800F53DE * 25 - 2225)) {
        v0 = D_800F594F;
        D_80148620 = (s16)((((u32)v0 < 1) ^ 1) * 3 + 4);
        D_800F594F = (u8)(v0 + 1);
        func_8004DE1C();
    } else {
        D_80148620 += 1;
        D_800F54B2 += 10;
        func_8008FB00();
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
    }
}
