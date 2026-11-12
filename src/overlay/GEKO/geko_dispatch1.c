/* @PostProc: patch_stack_complete --old 104 --new 120 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x801338F0 */
/* Size: 124 bytes (31 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[19];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable geko_dispatch1_tbl;

void geko_dispatch1(void)
{
    JumpTable table;

    table = geko_dispatch1_tbl;

    table.ptrs[now_sub_sub_step](128);
}
