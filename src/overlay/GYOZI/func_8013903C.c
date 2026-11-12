/* @PostProc: patch_stack_complete --old 104 --new 128 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x8013903C */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[20];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_801476A4;

void func_8013903C(void)
{
    JumpTable table;

    table = D_801476A4;

    table.ptrs[D_800F647A]();
}
