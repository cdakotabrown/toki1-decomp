/* @PostProc: patch_stack_complete --old 128 --new 152 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x8013A2CC */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[26];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable shougatu_subsub_jump7_tbl;

void shougatu_girl5_kind0_step(void)
{
    JumpTable table;

    table = shougatu_subsub_jump7_tbl;

    table.ptrs[now_sub_sub_step]();
}
