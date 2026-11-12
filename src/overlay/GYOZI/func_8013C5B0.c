/* @PostProc: patch_stack_complete --old 96 --new 112 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x8013C5B0 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[17];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_80148180;

void func_8013C5B0(void)
{
    JumpTable table;

    table = D_80148180;

    table.ptrs[D_800F647A](128);
}
