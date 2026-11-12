/* @PostProc: patch_stack_complete --old 216 --new 232 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x80134B50 */
/* Size: 164 bytes (41 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[47];
} JumpTable;

extern u8 D_800B1AF6;
extern JumpTable D_801209B8;

extern void func_800F8C2C(void);

void func_80134B50(void)
{
    JumpTable table;

    table = D_801209B8;

    table.ptrs[D_800B1AF6](128);

    if (D_800B1AF6 == 13) {
        func_800F8C2C();
    }
}
