/* @PostProc: patch_stack_complete --old 120 --new 136 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x80133D10 */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[23];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable shougatu_main53d_tbl;

void shougatu_main53d(void)
{
    JumpTable table;

    table = shougatu_main53d_tbl;

    table.ptrs[now_sub_sub_step](0x80);
}
