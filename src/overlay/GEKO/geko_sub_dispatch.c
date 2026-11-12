/* @PostProc: patch_stack_complete --old 152 --new 176 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x80142904 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[32];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable geko_sub_dispatch_tbl;

void geko_sub_dispatch(void)
{
    JumpTable table;

    table = geko_sub_dispatch_tbl;

    table.ptrs[now_sub_sub_step](128);
}
