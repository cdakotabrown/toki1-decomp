/* @PostProc: patch_stack_complete --old 120 --new 136 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x80133900 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[23];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_80146060;

void func_80133900(void)
{
    JumpTable table;

    table = D_80146060;

    table.ptrs[D_800F647A](128);
}
