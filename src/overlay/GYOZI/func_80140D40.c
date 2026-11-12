/* @PostProc: swap_branch_operands --offsets 0x90,0x9c */
/* @PostProc: patch_insn_reg_field 0x74:rt:t4:v1 0x78:rs:t4:v1 0x78:rt:t4:v1 0x7c:rt:t8:t5 0x80:rd:t5:t4 0x80:rt:t4:v1 0x84:rt:t5:t4 0x8c:rs:t8:t5 0x8c:rt:t8:t5 0x90:rs:t8:t5 0x94:rt:t7:t8 0x98:rs:t7:t8 0x98:rt:t7:t8 0x9c:rs:t7:t8 0xa0:rt:t6:t7 0xa4:rs:t6:t7 0xa4:rt:t6:t7 0xac:rt:t6:t7 */
/* Address: 0x80140D40, Size: 176 bytes (44 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[27];
} JumpTable;

extern void func_80089B6C(s32);
extern void func_80089C84(s32);

extern u8 D_800F647A;
extern u8 D_801317EB;
extern u8 D_8012B8C7;
extern JumpTable D_80148904;

void func_80140D40(void)
{
    s32 pad_hi;
    JumpTable table;
    FuncPtr fn;

    table = D_80148904;

    table.ptrs[D_800F647A](128);

    fn = table.ptrs[D_800F647A];
    if (fn == func_80089B6C || fn == func_80089C84) {
        D_8012B8C7 = D_801317EB;
    }
}
