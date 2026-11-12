/* @PostProc: patch_stack_complete --old 104 --new 128 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x80139A18 */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[20];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable shougatu_subsub_jump4_tbl;

void shougatu_girl1_kind1_step(void)
{
    JumpTable table;

    table = shougatu_subsub_jump4_tbl;

    table.ptrs[now_sub_sub_step]();
}
