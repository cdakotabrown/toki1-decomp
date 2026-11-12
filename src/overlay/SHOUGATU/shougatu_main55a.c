/* @PostProc: patch_stack_complete --old 128 --new 144 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x8013B6B0 */
/* Size: 124 bytes (31 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[25];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable shougatu_main55a_tbl;

void shougatu_main55a(void)
{
    JumpTable table;

    table = shougatu_main55a_tbl;

    table.ptrs[now_sub_sub_step](128);
}
