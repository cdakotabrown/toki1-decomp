/* @PostProc: patch_stack_complete --old 192 --new 208 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x8013B218 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[41];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable geko_sub_sub_dispatch41_tbl;

void geko_sub_sub_dispatch41(void)
{
    JumpTable table;

    table = geko_sub_sub_dispatch41_tbl;

    table.ptrs[now_sub_sub_step](128);
}
