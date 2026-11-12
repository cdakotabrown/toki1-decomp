/* @PostProc: patch_stack_complete --old 104 --new 128 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x801399FC */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[20];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_80147908;

void func_801399FC(void)
{
    JumpTable table;

    table = D_80147908;

    table.ptrs[D_800F647A]();
}
