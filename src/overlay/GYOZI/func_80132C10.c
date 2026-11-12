/* @PostProc: patch_stack_complete --old 120 --new 136 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x80132C10 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[23];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_80145F00;

void func_80132C10(void)
{
    JumpTable table;

    table = D_80145F00;

    table.ptrs[D_800F647A](128);
}
