/* @PostProc: patch_stack_complete --old 216 --new 232 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x801445C0 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[47];
} JumpTable;

extern u8 D_800B1AF6;
extern JumpTable D_80123990;

void func_801445C0(void)
{
    JumpTable table;

    table = D_80123990;

    table.ptrs[D_800B1AF6](128);
}
