/* @SymbolSize: 124 */
/* @PostProc: patch_stack_complete --old 104 --new 120 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x8013BB00 */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[19];
} JumpTable8013BB00;

extern JumpTable8013BB00 shougatu_main55b_tbl;
extern u8 now_sub_sub_step;

void shougatu_main55b(void)
{
    JumpTable8013BB00 table;
    s32 idx;

    table = shougatu_main55b_tbl;
    idx = (s32)now_sub_sub_step;
    table.ptrs[idx](128);
}
