/* @PostProc: patch_stack_complete --old 120 --new 136 */
/* @PostProc: remap_stack_slots --from 32 --to 44 */
/* @SymbolSize: 164 */
/* Address: 0x801365C0, Size: 164 bytes (41 insns) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[22];
} JumpTable;

extern u8 D_800B1AF6;
extern JumpTable D_80120DCC;
extern void func_8003C2C0(void);

void func_801365C0(void)
{
    JumpTable table;
    s32 idx;

    table = D_80120DCC;

    table.ptrs[D_800B1AF6]();

    idx = D_800B1AF6;
    if ((u32)idx < 13U) {
    } else if ((u32)idx < 18U) {
        func_8003C2C0();
    }
}
