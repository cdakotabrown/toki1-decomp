/* @PostProc: patch_stack_complete --old 112 --new 136 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x8013CB9C */
/* Size: 124 bytes (31 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[22];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable geko_sub_sub_dispatch22_tbl;

void geko_sub_sub_dispatch22(void)
{
    JumpTable table;

    table = geko_sub_sub_dispatch22_tbl;

    table.ptrs[now_sub_sub_step](128);
}
