/* @PostProc: patch_stack_complete --old 104 --new 120 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* @SymbolSize: 124 */
/* func_8013AEB0 -- GYOZI overlay, Address: 0x8013AEB0, Size: 124 bytes (31 insns) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[19];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_80147D64;

void func_8013AEB0(void)
{
    JumpTable table;

    table = D_80147D64;

    table.ptrs[D_800F647A]();
}
