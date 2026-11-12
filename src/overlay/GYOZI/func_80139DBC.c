/* func_80139DBC -- GYOZI overlay, Address: 0x80139DBC, Size: 228 bytes. */
/* @SymbolStart: 0x80139DA0 */
/* @SymbolSize: 256 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=a0:0 */
/* @PostProc: patch_insn_reg_field 0x10:rt:a0:t6 0x14:rs:a0:t6 0x14:rt:a0:t6 0x18:rt:a2:a1 0x1c:rs:a0:t6 0x1c:rt:a2:a1 0x20:rt:a1:a0 0x24:rs:a1:a0 0x24:rt:a1:a0 0x28:rt:t9:t1 0x38:rt:t1:t3 0x3c:rd:t6:t7 0x40:rs:a1:a0 0x40:rt:t6:t7 0x44:rt:t7:t9 0x4c:rs:t7:t9 0x4c:rt:t8:t0 0x50:rt:t8:t0 0x54:rs:t9:t1 0x54:rt:t9:t1 0x5c:rd:t0:t2 0x5c:rt:t9:t1 0x60:rt:t0:t2 0x70:rs:t1:t3 0x70:rt:t1:t3 0x78:rd:t2:t4 0x78:rt:t1:t3 0x7c:rt:t2:t4 0x9c:rt:t3:t5 0xa0:rs:t3:t5 0xa0:rt:t3:t5 0xa8:rs:t3:t5 0xa8:rt:t4:t6 0xb0:rt:t4:t6 0xc0:rt:a0:t9 0xc4:rs:a0:t9 0xc4:rt:a0:t9 0xc8:rt:t5:t7 0xcc:rs:a0:t9 0xcc:rt:a2:a1 0xd0:rs:t5:t7 0xd0:rt:t5:t7 0xd8:rs:t5:t7 0xd8:rt:t6:t8 0xdc:rt:t6:t8 0xe0:rd:t7:t0 0xe4:rs:a1:a0 0xe4:rt:t7:t0 0xe8:rt:t8:t1 0xf0:rs:t8:t1 0xf0:rt:t9:t2 0xf4:rt:t9:t2 */
#include "types.h"

extern volatile u8 D_800F62CF;
extern u8 D_800F53A0[][56];
extern u32 D_800F5488;
extern u8 D_800F5602;
extern s16 D_801474A8;

extern void func_8004DE1C(void);

void func_80139DBC(void)
{
    s32 c1;
    s32 c2;

    D_800F53A0[D_800F62CF][440] |= 4;

    if ((D_800F5488 >> 28) == 4) {
        c1 = 1;
    } else {
        c1 = 0;
    }
    if (((u32)D_800F5602 >> 4) == 4) {
        c2 = 1;
    } else {
        c2 = 0;
    }

    if (c1 == 0 && c2 != 0) {
        D_801474A8 += 2;
    } else if (c1 == 0 && c2 == 0) {
        D_801474A8 += 4;
        D_800F53A0[D_800F62CF][440] &= ~4;
    }

    func_8004DE1C();
}
