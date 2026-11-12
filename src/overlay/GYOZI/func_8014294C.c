/* @PostProc: patch_insn_reg_field 0x20:rt:v1:a0 0x24:rs:v1:a0 0x24:rt:v1:a0 0x2c:rs:v1:a0 0x2c:rt:v0:v1 0x30:rs:v1:a0 0x30:rt:v1:a0 0x34:rs:v0:v1 0x38:rt:v1:a0 0x4c:rd:t9:t0 0x50:rd:t8:t9 0x54:rt:t7:t8 0x58:rs:t8:t9 0x5c:rs:t8:t9 0x5c:rt:t8:t9 0x60:rs:t9:t0 0x64:rs:t8:t9 0x68:rs:t9:t0 0x68:rt:t9:t0 0x6c:rs:t9:t0 0x70:rs:t8:t9 0x74:rs:t8:t9 0x74:rt:t7:t8 0x78:rs:t9:t0 0x7c:rs:t8:t9 0x80:rd:t2:t4 0x84:rs:t9:t0 0x88:rs:t8:t9 0x88:rt:t7:t8 0x8c:rd:t3:t5 0x90:rt:t1:t3 0x94:rs:t9:t0 0x94:rt:t7:t8 0x98:rs:t2:t4 0x9c:rs:t2:t4 0x9c:rt:t2:t4 0xa0:rs:t3:t5 0xa4:rs:t2:t4 0xa8:rs:t3:t5 0xa8:rt:t3:t5 0xac:rs:t3:t5 0xb0:rs:t2:t4 0xb4:rs:t2:t4 0xb4:rt:t1:t3 0xb8:rs:t3:t5 0xbc:rs:t2:t4 0xc0:rt:t4:t7 0xc4:rs:t3:t5 0xc8:rs:t2:t4 0xc8:rt:t1:t3 0xd0:rs:t3:t5 0xd0:rt:t1:t3 0xd4:rt:t4:t7 0xd8:rt:t5:t6 0xdc:rs:t5:t6 0xdc:rt:t5:t6 0xe4:rs:t5:t6 0xe8:rt:t6:t8 0xec:rs:t6:t8 0xec:rt:t6:t8 0xf4:rs:t6:t8 0x108:rt:t7:t9 0x114:rt:t7:t9 */
/* @SymbolSize: 296 */
/* func_8014294C -- GYOZI overlay, Address: 0x8014294C, Size: 296 bytes (74 insns) */
#include "types.h"

typedef struct {
    s32 words[17];
} Struct68;

extern void func_80140050(void);
extern s32 func_8005B32C(void);

extern u16 D_800D9250;
extern u16 D_800D9254;
extern s32 D_800F6474;
extern s8 D_8012B94A;
extern s16 D_8012B95E;
extern s16 D_8012B960;
extern Struct68 D_8012B8C0;
extern Struct68 D_8012B948;

void func_8014294C(void)
{
    s32 frameCount;
    s32 isFirst;

    func_80140050();

    frameCount = D_800F6474;
    isFirst = (u32)frameCount < 1;
    D_800F6474 = frameCount + 1;

    if (isFirst) {
        D_8012B948 = D_8012B8C0;
        D_8012B948 = D_8012B8C0;
        D_8012B95E = 4;
    }

    if (D_800D9250 == 7 && D_800D9254 == 1 && func_8005B32C() == 0) {
        D_8012B94A = 1;
    } else {
        D_8012B94A = 0;
        D_8012B960 = 0;
    }
}
