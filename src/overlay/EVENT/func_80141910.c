/* @PostProc: patch_stack_complete --old 104 --new 128 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x80141910 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[20];
} JumpTable;

extern u8 D_800B1AF6;
extern JumpTable D_80123100;

void func_80141910(void)
{
    JumpTable table;

    table = D_80123100;

    table.ptrs[D_800B1AF6](128);
}
