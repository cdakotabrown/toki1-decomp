/* Address: 0x80134888 (true entry 0x80134878 -- cross-prologue head) */
/* Size: 232 bytes (58 instructions) + 16-byte head = 248 absorbed */
/* @SymbolStart: 0x80134878 */
/* @SymbolSize: 248 */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=occ:2,4 */
/* @PostProc: swap_instructions --offset1 0xbc --offset2 0xc0 */
/* @PostProc: patch_insn_reg_field 0x3c:rt:t1:v1 0x48:rt:t2:t1 0x4c:rt:v1:v0 0x50:rs:v1:v0 0x50:rt:v1:v0 0x58:rs:v1:v0 0x58:rt:v1:v0 0x5c:rt:v1:v0 0x60:rd:v1:v0 0x64:rs:v1:v0 0x70:rd:v1:v0 0x74:rs:t1:v1 0x74:rt:t1:v1 0x78:rs:t2:t1 0x78:rt:t2:t1 0x80:rd:v0:v1 0x80:rs:t1:v1 0x80:rt:t2:t1 0x84:rd:t3:t2 0x84:rt:v0:v1 0x88:rd:v0:v1 0x88:rt:t3:t2 0x8c:rs:v0:v1 0x94:rt:t5:t4 0x9c:rt:t5:t4 0xa4:rt:v0:v1 0xac:rs:v0:v1 0xb0:rt:t6:t5 0xb8:rt:t6:t5 0xc4:rt:v1:v0 0xcc:rt:v1:v0 0xd0:rt:t7:v1 0xd4:rs:t7:v1 0xd4:rt:t7:v1 0xe0:rs:t7:v1 0xe0:rt:t8:t6 0xe4:rt:t8:t6 */
#include "types.h"

extern u32 D_800B1AE4;
extern u8 D_800B1AF6;
extern s32 D_801206F8;
extern volatile s16 D_801207AC;
extern volatile s16 D_801207B0;
extern volatile s16 D_801207B4;

extern void func_80012D64(s32, s32, s32, s32, s32);

void func_80134888(void) {
    s16 counter;
    s16 sum;

    if ((D_800B1AE4 % 10) != 0) {
        return;
    }

    if (D_800B1AF6 < 13U) {
        return;
    }

    counter = D_801207AC + 1;
    if (counter >= 4) {
        counter = 0;
    }

    sum = D_801207B0 + D_801207B4;
    if (sum == 0) {
        D_801207B4 = 1;
    }
    D_801207B0 = sum;
    if (sum == 3) {
        D_801207B4 = -1;
    }

    D_801207AC = counter;
    func_80012D64(D_801206F8, 19, 1, 2, counter);
    func_80012D64(D_801206F8, 18, 1, 2, D_801207B0 + 4);
}
