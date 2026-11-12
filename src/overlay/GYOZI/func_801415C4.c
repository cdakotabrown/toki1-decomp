/* @SymbolSize: 136 */
/* @PostProc: patch_stack_complete --old 80 --new 104 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* func_801415C4 -- GYOZI overlay, Address: 0x801415C4, Size: 136 bytes (34 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[14];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_80148A14;

void func_801415C4(void)
{
    JumpTable t = D_80148A14;
    t.ptrs[D_800F647A](128);
}
