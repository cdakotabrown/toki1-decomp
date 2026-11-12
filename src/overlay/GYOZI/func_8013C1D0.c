/* @PostProc: patch_stack_complete --old 128 --new 144 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* @SymbolSize: 124 */
/* func_8013C1D0 -- GYOZI overlay, Address: 0x8013C1D0, Size: 124 bytes (31 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[25];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_80148114;

void func_8013C1D0(void)
{
    JumpTable table;

    table = D_80148114;

    table.ptrs[D_800F647A](128);
}
