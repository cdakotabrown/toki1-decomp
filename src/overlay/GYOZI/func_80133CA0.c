/* Address: 0x80133CA0, Size: 200 bytes (50 insns) */
/* @SymbolSize: 196 */
/* @PostProc: patch_stack_complete --old 136 --new 152 */
/* @PostProc: remap_stack_slots --map '36:48' */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x68 */
/* @PostProc: patch_insn_reg_field 0x18:rt:a1:a2 0x28:rs:a1:a2 0x60:rt:t2:a1 0x6c:rs:t2:a1 0x6c:rt:t2:a1 0x70:rt:t4:t3 0x74:rd:t3:t2 0x74:rt:t2:a1 0x78:rs:a1:a2 0x78:rt:t3:t2 0x80:rs:t4:t3 0x80:rt:t4:t3 0x84:rs:v1:t3 0x84:rt:t4:v1 0x88:rt:t5:t4 0x8c:rs:t5:t4 0x8c:rt:t5:t4 0x90:rs:v1:t4 0x90:rt:t5:v1 0x94:rt:t8:t5 0x98:rs:t8:t5 0x98:rt:t8:t5 0x9c:rt:t8:t5 0xa0:rt:t7:a1 0xa4:rs:t7:a1 0xa4:rt:t7:a1 0xac:rd:t9:t8 0xac:rt:t7:a1 0xb0:rd:t6:t7 0xb0:rs:a1:a2 0xb0:rt:t9:t8 0xb4:rs:t6:t7 */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[25];
} JumpTable25;

extern volatile u8 D_800F647A;
extern u8 D_80145F60;
extern JumpTable25 D_801460E0;
extern void func_800896E0(s32);
extern void func_800898F0(s32);
extern void func_8004DE1C(s32);

void func_80133CA0(void)
{
    JumpTable25 table;
    FuncPtr *slot;
    FuncPtr cur;

    table = D_801460E0;

    if (D_80145F60 != 0) {
        slot = &table.ptrs[D_800F647A];
        cur = *slot;
        if (cur == (FuncPtr)func_800896E0 || cur == (FuncPtr)func_800898F0) {
            *slot = (FuncPtr)func_8004DE1C;
        }
    }

    table.ptrs[D_800F647A](128);
}
