/* @PostProc: patch_stack_complete --old 264 --new 288 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* @SymbolSize: 116 */
/* func_80132FD0 -- GYOZI overlay, Address: 0x80132FD0, Size: 116 bytes (29 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[60];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_80145F68;

void func_80132FD0(void)
{
    JumpTable table;

    table = D_80145F68;

    table.ptrs[D_800F647A](128);
}
