/* @PostProc: patch_insn_reg_field 0x28:rt:t6:t7 0x2c:rs:t6:t7 0x2c:rt:t6:t7 0x34:rs:t6:t7 0x38:rt:v0:t6 0x3c:rt:t7:t8 0x40:rs:t7:t8 0x40:rt:t7:t8 0x44:rs:v0:t6 0x44:rt:t8:t9 0x48:rs:t8:t9 0x48:rt:t8:t9 0x4c:rs:v0:t6 0x4c:rt:t0:t1 0x50:rs:t0:t1 0x50:rt:t0:t1 0x54:rd:t9:t0 0x54:rs:t7:t8 0x54:rt:t8:t9 0x58:rd:t1:t2 0x58:rs:t9:t0 0x58:rt:t0:t1 0x64:rt:t1:t2 0x68:rt:t2:t3 0x6c:rs:t2:t3 0x6c:rt:t2:t3 0x74:rs:t2:t3 0x74:rt:t3:t4 0x7c:rt:t3:t4 */
/* Address: 0x80152620 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

extern s32 func_8002328C(s32 id);
extern void func_80011DFC(void);

extern u8 D_800B1746;
extern u8 D_8012531C;
extern s16 D_80094730;
extern u8 D_800B1AF6;

void func_80152620(void)
{
    s32 v;

    v = func_8002328C(D_800B1746);

    if (D_8012531C == 5) {
        s32 lo;

        lo = v & 0x7F;
        D_80094730 = (s16)(D_80094730 + ((u32)lo >= 2) + ((u32)lo >= 3));
        func_80011DFC();
        D_800B1AF6 = D_800B1AF6 + 8;
    } else {
        func_80011DFC();
    }
}
