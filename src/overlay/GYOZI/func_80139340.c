/* @PostProc: patch_stack_complete --old 120 --new 144 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* func_80139340 -- GYOZI overlay, Address: 0x80139340, Size: 116 bytes (29 insns) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[24];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_801477F8;

void func_80139340(void)
{
    JumpTable table;

    table = D_801477F8;

    table.ptrs[D_800F647A]();
}
