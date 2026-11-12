/* @PostProc: patch_stack_complete --old 192 --new 216 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* func_8013BB70 -- GYOZI overlay, Address: 0x8013BB70, Size: 116 bytes (29 insns) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[42];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_80147FB0;

void func_8013BB70(void)
{
    JumpTable table;

    table = D_80147FB0;

    table.ptrs[D_800F647A]();
}
