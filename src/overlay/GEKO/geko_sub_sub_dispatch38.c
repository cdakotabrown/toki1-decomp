/* @PostProc: patch_stack_complete --old 176 --new 200 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x8013C9D0 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[38];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable geko_sub_sub_dispatch38_tbl;

void geko_sub_sub_dispatch38(void)
{
    JumpTable table;

    table = geko_sub_sub_dispatch38_tbl;

    table.ptrs[now_sub_sub_step](128);
}
