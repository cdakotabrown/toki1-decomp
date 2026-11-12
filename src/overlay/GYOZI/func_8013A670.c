/* @PostProc: patch_stack_complete --old 128 --new 144 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* @SymbolSize: 124 */
/* func_8013A670 -- GYOZI overlay, Address: 0x8013A670, Size: 124 bytes (31 insns) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[25];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_80147BB8;

void func_8013A670(void)
{
    JumpTable table;

    table = D_80147BB8;

    table.ptrs[D_800F647A]();
}
