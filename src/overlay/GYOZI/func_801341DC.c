/* @SymbolStart: 0x801341D4 */
/* @SymbolSize: 64 */
/* @PostProc: patch_insn_reg_field 0x10:rt:t6:a0 0x14:rs:t6:a0 0x14:rt:t6:a0 0x28:rt:t6:a0 0x2c:rt:t7:t6 0x30:rs:t7:t6 0x30:rt:t7:t6 0x3c:rt:t7:t6 */
/* func_801341DC -- GYOZI overlay, Address: 0x801341D4, Size: 64 bytes (16 insns) */
#include "types.h"

extern void func_8008F618(void);
extern void func_8004DE1C(void);

extern u8 D_800F5ACD;
extern s8 D_800F62CF;
extern s32 D_8012E67C;
extern s32 D_8012E66C;

void func_801341DC(void)
{
    u32 temp = D_800F5ACD;

    D_800F62CF = (s8)temp;
    func_8008F618();
    D_8012E66C = D_8012E67C;
    func_8004DE1C();
}
