/* @PostProc: patch_stack_complete --old 64 --new 88 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* @SymbolSize: 124 */
/* func_80141520 -- GYOZI overlay, Address: 0x80141520, Size: 124 bytes (31 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[10];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_801489EC;

void func_80141520(void)
{
    JumpTable table;

    table = D_801489EC;

    table.ptrs[D_800F647A](128);
}
