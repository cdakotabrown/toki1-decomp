/* @PostProc: patch_stack_complete --old 64 --new 80 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* func_80137920 -- GYOZI overlay, Address: 0x80137920, Size: 116 bytes (29 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[9];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_80147330;

void func_80137920(void)
{
    JumpTable table;

    table = D_80147330;

    table.ptrs[D_800F647A](128);
}
