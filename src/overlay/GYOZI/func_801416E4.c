/* @PostProc: patch_stack_complete --old 72 --new 96 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* func_801416E4 -- GYOZI overlay, Address: 0x801416E4, Size: 116 bytes (29 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[12];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_80148A4C;

void func_801416E4(void)
{
    JumpTable table;

    table = D_80148A4C;

    table.ptrs[D_800F647A](128);
}
