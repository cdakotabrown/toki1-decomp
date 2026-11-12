/* @PostProc: patch_stack_complete --old 80 --new 96 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* @SymbolSize: 124 */
/* func_80132520 -- GYOZI overlay, Address: 0x80132520, Size: 124 bytes (31 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[13];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_80145ECC;

void func_80132520(void)
{
    JumpTable table;

    table = D_80145ECC;

    table.ptrs[D_800F647A](128);
}
