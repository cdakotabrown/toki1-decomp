/* @PostProc: patch_stack_complete --old 80 --new 96 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x80132550 */
/* Size: 124 bytes (31 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[13];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable shougatu_main53a_tbl;

void shougatu_main53a(void)
{
    JumpTable table;

    table = shougatu_main53a_tbl;

    table.ptrs[now_sub_sub_step](0x80);
}
