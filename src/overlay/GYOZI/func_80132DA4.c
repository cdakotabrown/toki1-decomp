/* @SymbolStart: 0x80132D9C */
/* @SymbolSize: 260 */
/* @PostProc: patch_insn_reg_field 0x2c:rt:a1:a0 0x30:rt:a1:a0 0x3c:rt:a0:v1 0x40:rs:a0:v1 0x40:rt:a0:v1 0x5c:rs:a0:v1 0x78:rt:a1:a0 0x80:rs:a0:v1 0x9c:rt:a1:a0 0xa4:rs:a0:v1 0xbc:rt:v1:t1 0xc0:rs:v1:t1 0xc0:rt:v1:t1 0xc8:rs:v1:t1 0xd0:rs:v1:t1 0xd0:rt:a1:a0 0xd4:rt:a0:v1 0xd8:rs:a0:v1 0xd8:rt:a0:v1 0xdc:rd:t1:t2 0xe0:rs:a0:v1 0xe0:rt:t1:t2 0xe4:rt:t2:t3 0xec:rs:t2:t3 0xec:rt:t3:t4 0xf0:rt:t3:t4 */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xcc */
/* @PostProc: patch_insn_reg_field 0xcc:rt:a1:a0 */
/* Function: func_80132DA4 (true entry 0x80132D9C) */
/* Overlay: GYOZI */
#include "types.h"

extern void func_8008FB00(void);
extern void func_8004DE1C(void);

extern s32 D_8012E66C;
extern volatile u8 D_800F62CF;
extern s16 D_80145EB4;
extern u8 D_800F53A0[];

void func_80132DA4(void)
{
    u8 *p;

    if (D_8012E66C != 0) {
        D_80145EB4 = D_80145EB4 + 1;
        p = D_800F53A0 + D_800F62CF * 56;
        *(s16 *)(p + 434) = *(s16 *)(p + 434) - 1;
        p = D_800F53A0 + D_800F62CF * 56;
        *(s16 *)(p + 430) = *(s16 *)(p + 430) - 1;
        p = D_800F53A0 + D_800F62CF * 56;
        *(s16 *)(p + 438) = *(s16 *)(p + 438) + 20;
    } else {
        s32 idx = D_800F62CF;
        if (idx == 0) {
            p = D_800F53A0 + idx * 56;
            *(s16 *)(p + 430) = *(s16 *)(p + 430) + 1;
        }
    }
    func_8008FB00();
    func_8004DE1C();
}
