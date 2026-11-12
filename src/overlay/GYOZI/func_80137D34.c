/* @PostProc: patch_stack_complete --old 72 --new 96 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* func_80137D34 -- GYOZI overlay, Address: 0x80137D34, Size: 116 bytes (29 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[12];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_801473FC;

void func_80137D34(void)
{
    JumpTable table;

    table = D_801473FC;

    table.ptrs[D_800F647A](128);
}
