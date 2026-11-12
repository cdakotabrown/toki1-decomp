/* @PostProc: patch_stack_complete --old 120 --new 136 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x80138998 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[23];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_801474C0;

void func_80138998(void)
{
    JumpTable table;

    table = D_801474C0;

    table.ptrs[D_800F647A]();
}
