/* @PostProc: patch_stack_complete --old 200 --new 224 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x80137728 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[44];
} JumpTable;

extern u8 D_800B1AF6;
extern JumpTable D_80120F94;

void func_80137728(void)
{
    JumpTable table;

    table = D_80120F94;

    table.ptrs[D_800B1AF6]();
}
