/* @PostProc: patch_stack_complete --old 144 --new 160 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x8013843C */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[29];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable shougatu_step_dispatch5_tbl;

void shougatu_step_dispatch5(void)
{
    JumpTable table;

    table = shougatu_step_dispatch5_tbl;

    table.ptrs[now_sub_sub_step]();
}
