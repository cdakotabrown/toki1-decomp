/* @PostProc: patch_stack_complete --old 168 --new 184 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x80143090 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[35];
} JumpTable;

extern u8 D_800B1AF6;
extern JumpTable D_8012350C;

void func_80143090(void)
{
    JumpTable table;

    table = D_8012350C;

    table.ptrs[D_800B1AF6]();
}
