/* @PostProc: patch_stack_complete --old 104 --new 128 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x8013B6B4 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[20];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_80147F54;

void func_8013B6B4(void)
{
    JumpTable table;

    table = D_80147F54;

    table.ptrs[D_800F647A]();
}
