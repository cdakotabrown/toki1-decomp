/* @PostProc: patch_stack_complete --old 144 --new 160 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x801392B8 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[29];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_80147784;

void func_801392B8(void)
{
    JumpTable table;

    table = D_80147784;

    table.ptrs[D_800F647A]();
}
