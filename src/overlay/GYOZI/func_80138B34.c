/* @PostProc: patch_stack_complete --old 120 --new 136 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x80138B34 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[23];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_80147578;

void func_80138B34(void)
{
    JumpTable table;

    table = D_80147578;

    table.ptrs[D_800F647A]();
}
