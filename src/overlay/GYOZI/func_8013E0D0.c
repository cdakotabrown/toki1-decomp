/* @PostProc: patch_stack_complete --old 160 --new 176 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* @SymbolSize: 144 */
/* Address: 0x8013E0D0, Size: 144 bytes (36 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[33];
} JumpTable;

extern u8 D_800F647A;
extern u8 D_800F594E;
extern u8 D_800F594F;
extern JumpTable D_80148640;
extern u8 D_80145CF0[];
extern void func_800BCDF0(u8 *, u8, u8);

void func_8013E0D0(void)
{
    JumpTable table;

    table = D_80148640;

    table.ptrs[D_800F647A](128);

    func_800BCDF0(D_80145CF0, D_800F594E, D_800F594F);
}
