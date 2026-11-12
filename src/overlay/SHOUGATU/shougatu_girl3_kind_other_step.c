/* @PostProc: patch_stack_complete --old 104 --new 128 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x80138CB4 */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[20];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable shougatu_subsub_jump2_tbl;

void shougatu_girl3_kind_other_step(void)
{
    JumpTable table;

    table = shougatu_subsub_jump2_tbl;

    table.ptrs[now_sub_sub_step]();
}
