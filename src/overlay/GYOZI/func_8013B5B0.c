/* @PostProc: patch_stack_complete --old 176 --new 192 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* @SymbolSize: 124 */
/* func_8013B5B0 -- GYOZI overlay, Address: 0x8013B5B0, Size: 124 bytes (31 insns) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[37];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_80147E70;

void func_8013B5B0(void)
{
    JumpTable table;

    table = D_80147E70;

    table.ptrs[D_800F647A]();
}
