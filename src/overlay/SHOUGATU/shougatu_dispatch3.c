/* @PostProc: patch_stack_complete --old 64 --new 88 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x80141250 */
/* Size: 124 bytes (31 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[10];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable shougatu_dispatch3_tbl;

void shougatu_dispatch3(void)
{
    JumpTable table;

    table = shougatu_dispatch3_tbl;

    table.ptrs[now_sub_sub_step](128);
}
