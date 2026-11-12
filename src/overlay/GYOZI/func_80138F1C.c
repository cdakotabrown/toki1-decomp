/* @PostProc: patch_stack_complete --old 112 --new 128 */
/* @PostProc: remap_stack_slots --from 32 --to 44 */
/* Address: 0x80138F1C */
/* Size: 204 bytes (51 instructions) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[20];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_80147654;
extern s16 D_801474A8;
extern s16 D_801474AC;
extern u32 D_800F6474;

extern void func_80086AB0(s32);

void func_80138F1C(void)
{
    JumpTable table;
    s32 wasZero;

    table = D_80147654;

    table.ptrs[D_800F647A]();

    if (D_801474A8 != 1) return;
    if (D_801474AC != 8) return;
    wasZero = (D_800F6474 < 1U);
    D_800F6474 += 1;
    if (wasZero) {
        func_80086AB0(1280);
    }
}
