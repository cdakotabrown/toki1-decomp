/* @PostProc: patch_stack_complete --old 232 --new 256 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x801337A0 */
/* Size: 132 bytes (33 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[52];
} JumpTable;

extern u8 D_800B1AF6;
extern JumpTable D_801207B8;
extern s8 D_800EAFA2;

void func_801337A0(void)
{
    JumpTable table;

    table = D_801207B8;

    table.ptrs[D_800B1AF6](128);
    D_800EAFA2 = 1;
}
