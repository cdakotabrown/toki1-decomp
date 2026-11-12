/* @PostProc: patch_stack_complete --old 96 --new 112 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x80141780 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[17];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_80148A7C;

void func_80141780(void)
{
    JumpTable table;

    table = D_80148A7C;

    table.ptrs[D_800F647A](128);
}
