/* @PostProc: patch_stack_complete --old 232 --new 248 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* @SymbolSize: 124 */
/* func_801427F0 -- GYOZI overlay, Address: 0x801427F0, Size: 124 bytes (31 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[51];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_80148F50;
extern void func_80144B70(void);

void func_801427F0(void)
{
    JumpTable table;

    table = D_80148F50;

    table.ptrs[D_800F647A](128);

    func_80144B70();
}
