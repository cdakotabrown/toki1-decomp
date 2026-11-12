/* @PostProc: patch_stack_complete --old 80 --new 104 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x801413E0 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[14];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_801489B4;

void func_801413E0(void)
{
    JumpTable table;

    table = D_801489B4;

    table.ptrs[D_800F647A](128);
}
