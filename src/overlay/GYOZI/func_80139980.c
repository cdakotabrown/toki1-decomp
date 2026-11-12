/* @PostProc: patch_stack_complete --old 112 --new 136 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* @SymbolSize: 124 */
/* func_80139980 -- GYOZI overlay, Address: 0x80139980, Size: 124 bytes (31 insns) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[22];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_801478B0;

void func_80139980(void)
{
    JumpTable table;

    table = D_801478B0;

    table.ptrs[D_800F647A]();
}
