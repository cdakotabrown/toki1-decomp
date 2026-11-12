/* @PostProc: patch_stack_complete --old 88 --new 112 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* @SymbolSize: 124 */
/* func_80141314 -- GYOZI overlay, Address: 0x80141314, Size: 124 bytes (31 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[16];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_80148974;

void func_80141314(void)
{
    JumpTable table;

    table = D_80148974;

    table.ptrs[D_800F647A](128);
}
