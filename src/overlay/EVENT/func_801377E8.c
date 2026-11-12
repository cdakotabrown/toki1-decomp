/* @PostProc: patch_stack_complete --old 256 --new 280 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x801377E8 */
/* Size: 124 bytes (31 instructions) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[58];
} JumpTable;

extern u8 D_800B1AF6;
extern JumpTable D_80121044;

void func_801377E8(void)
{
    JumpTable table;

    table = D_80121044;

    table.ptrs[D_800B1AF6]();
}
