/* @PostProc: patch_stack_complete --old 144 --new 160 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x80134414 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[29];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable geko_step_dispatch_tbl;

void geko_step_dispatch(void)
{
    JumpTable table;

    table = geko_step_dispatch_tbl;

    table.ptrs[now_sub_sub_step](128);
}
