/* @SymbolStart: 0x80156D7C */
/* @SymbolSize: 420 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x168 --offset2 0x16c */
/* @PostProc: swap_instructions --offset1 0xb0 --offset2 0xb4 */
/* @PostProc: swap_instructions --offset1 0xb4 --offset2 0xb8 */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xc8 */
/* @PostProc: swap_instructions --offset1 0x140 --offset2 0x148 */
/* @PostProc: patch_insn_reg_field 0x10:rt:a2:v1 0x14:rt:v0:a0 0x18:rs:v0:a0 0x1c:rs:a2:v1 0x2c:rt:a1:a2 0x30:rs:a1:a2 0x30:rt:a1:a2 0x38:rs:a1:a2 0x70:rt:a1:a2 0x74:rs:a1:a2 0x74:rt:a1:a2 0x80:rs:a1:a2 0xa4:rt:a1:a2 0xb0:rt:t5:a1 0xc0:rt:t5:a1 0xc4:rt:v1:a1 0xc8:rt:a0:t5 0xcc:rs:v1:a1 0xcc:rt:v1:a1 0xd0:rs:a0:t5 0xd4:rs:a0:t5 0xe0:rs:v1:a1 0xf0:rt:v1:a1 0xf4:rs:v1:a1 0xfc:rt:v1:a1 0x10c:rt:a1:a2 0x114:rs:a1:a2 0x114:rt:a1:a2 0x130:rs:a1:a2 0x134:rt:a2:v1 0x13c:rs:a2:v1 0x13c:rt:a2:v1 0x140:rt:t7:a0 0x144:rs:v0:v1 0x144:rt:a2:v0 0x14c:rs:t7:a0 0x14c:rt:t7:a0 0x158:rt:t7:a0 0x160:rt:a2:v1 0x164:rt:v0:a0 0x168:rs:v0:a0 0x168:rt:v0:a0 0x16c:rs:a2:v1 0x16c:rt:a2:v1 0x174:rs:a1:a2 0x178:rt:v1:v0 0x17c:rs:v1:v0 0x17c:rt:v1:v0 0x184:rs:a2:v1 0x184:rt:v1:v0 0x194:rt:v0:a0 0x198:rt:v1:v0 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=2 */
/* Address: 0x80156D8C */
/* Size: 404 bytes (101 instructions) */
#include "types.h"

extern u8 D_800B0F2C;
extern volatile u8 D_800B0F2D;
extern u8 D_800B0F2E;
extern u8 D_800B0F2F;
extern s32 D_800EECBC;
extern s16 D_80124344;
extern s16 D_80124384;
extern u8 D_801243B0;
extern u8 D_801243B4;
extern u8 D_801252E4;
extern u8 D_800B1AF6;
extern u8 D_800B1746;

extern u8 func_8002328C();
extern void func_80011DFC();

void func_80156D8C(void) {
    u32 rank;
    u8 sel;
    u8 cur;
    u8 nxt;

    cur = D_800B0F2C;
    nxt = D_800B0F2D;
    if (nxt == cur && D_800EECBC == 0) {
        D_801243B4 = 1;
        D_80124344 += 1;
        D_800B1AF6 += 13;
    } else if (nxt != cur && D_800EECBC == 0) {
        D_800B1AF6 += 30;
    } else {
        D_80124384 += D_800EECBC;
        D_801252E4 = 2;
        rank = func_8002328C(D_800B1746) & 0x7F;
        sel = D_801252E4;
        D_80124344 = ((rank >= 2U) + (rank >= 3U)) * ((sel == 0) + 1)
                     + sel * 6 - (sel == 2) * 3;
        if (D_800EECBC == 1) {
            if (D_800B0F2C == D_800B0F2E) {
                D_801243B0 = D_800B0F2D;
            } else {
                D_801243B0 = D_800B0F2E;
            }
        }
        cur = D_800B0F2C;
        nxt = D_800B0F2D;
        if (D_800EECBC == 2) {
            if (D_800B0F2F == cur) {
                D_801243B0 = nxt;
            } else {
                D_801243B0 = D_800B0F2F;
            }
        }
        func_80011DFC();
    }
}
