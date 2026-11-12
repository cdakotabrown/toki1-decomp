/* @PostProc: patch_stack_complete --old 128 --new 152 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x8013B15C */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[26];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_80147DB0;

void func_8013B15C(void)
{
    JumpTable table;

    table = D_80147DB0;

    table.ptrs[D_800F647A](128);
}
