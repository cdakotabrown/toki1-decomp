/* @PostProc: patch_stack_complete --old 248 --new 272 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x80136BF0 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[56];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_80147248;

void func_80136BF0(void)
{
    JumpTable table;

    table = D_80147248;

    table.ptrs[D_800F647A](128);
}
