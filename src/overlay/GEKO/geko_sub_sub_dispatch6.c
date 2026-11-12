/* @PostProc: patch_stack_complete --old 224 --new 248 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x8013BD84 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[50];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable geko_sub_sub_dispatch6_tbl;

void geko_sub_sub_dispatch6(void)
{
    JumpTable table;

    table = geko_sub_sub_dispatch6_tbl;

    table.ptrs[now_sub_sub_step](128);
}
