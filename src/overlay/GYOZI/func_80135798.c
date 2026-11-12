/* Address: 0x80135798, Size: 188 bytes (47 instructions) */
/* @PostProc: patch_stack_complete --old 224 --new 248 */
/* @PostProc: remap_stack_slots --map '32:52' */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x60 */
/* @PostProc: patch_insn_reg_field 0x18:rt:a1:a2 0x28:rs:a1:a2 0x58:rt:t2:a1 0x64:rs:t2:a1 0x64:rt:t2:a1 0x68:rt:t4:t3 0x6c:rd:t3:t2 0x6c:rt:t2:a1 0x70:rs:a1:a2 0x70:rt:t3:t2 0x78:rs:t4:t3 0x78:rt:t4:t3 0x7c:rs:v1:t3 0x7c:rt:t4:v1 0x80:rt:t5:t4 0x84:rs:t5:t4 0x84:rt:t5:t4 0x88:rs:v1:t4 0x88:rt:t5:v1 0x8c:rt:t8:t5 0x90:rs:t8:t5 0x90:rt:t8:t5 0x94:rt:t8:t5 0x98:rt:t7:a1 0x9c:rs:t7:a1 0x9c:rt:t7:a1 0xa4:rd:t9:t8 0xa4:rt:t7:a1 0xa8:rd:t6:t7 0xa8:rs:a1:a2 0xa8:rt:t9:t8 0xac:rs:t6:t7 */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[48];
} JumpTable;

extern volatile u8 D_800F647A;
extern u8 D_80145F60;
extern JumpTable D_80146368;
extern void func_800896E0(s32);
extern void func_800898F0(s32);
extern void func_8004DE1C(s32);

void func_80135798(void)
{
    JumpTable table;
    FuncPtr *slot;
    FuncPtr cur;

    table = D_80146368;

    if (D_80145F60 != 0) {
        slot = &table.ptrs[D_800F647A];
        cur = *slot;
        if (cur == (FuncPtr)func_800896E0 || cur == (FuncPtr)func_800898F0) {
            *slot = (FuncPtr)func_8004DE1C;
        }
    }

    table.ptrs[D_800F647A](128);
}
