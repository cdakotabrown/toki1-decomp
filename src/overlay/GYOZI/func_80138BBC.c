/* @PostProc: patch_stack_complete --old 136 --new 160 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* @SymbolSize: 124 */
/* func_80138BBC -- GYOZI overlay, Address: 0x80138BBC, Size: 124 bytes (31 insns) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[28];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_801475D4;

void func_80138BBC(void)
{
    JumpTable table;

    table = D_801475D4;

    table.ptrs[D_800F647A]();
}
