/* func_80134124 -- GYOZI overlay, Address: 0x80134124, Size: 132 bytes */
/* @SymbolSize: 132 */
/* @PostProc: patch_insn_reg_field 0x28:rt:a2:a1 0x2c:rs:a2:a1 0x2c:rt:a2:a1 0x34:rs:a2:a1 0x38:rt:a1:v1 0x3c:rs:a1:v1 0x3c:rt:a1:v1 0x44:rs:t6:v1 0x44:rt:a1:t6 0x4c:rs:a2:a1 0x5c:rd:v1:v0 0x5c:rs:t9:v1 0x5c:rt:a1:t9 0x60:rs:v1:v0 0x70:rs:v1:v0 */
#include "types.h"

extern void func_8008F618(s32);
extern void func_8008FB00(void);
extern void func_8004DE1C(void);

extern u8 D_800F53A0[];
extern s8 D_800F62CF;
extern s32 D_8012E66C;

void func_80134124(void)
{
    u8 *p;
    u32 g;

    func_8008F618(D_8012E66C);

    g = (u32)D_8012E66C;
    p = D_800F53A0;
    *(s16 *)(p + (g | 0U) * 56U + 430) =
        (s16)(*(s16 *)(p + (g | 0U) * 56U + 430) + 5);
    *(s16 *)(p + (g ^ 0U) * 56U + 434) =
        (s16)(*(s16 *)(p + (g ^ 0U) * 56U + 434) + 1);

    func_8008FB00();
    D_800F62CF = 14;
    func_8004DE1C();
}
