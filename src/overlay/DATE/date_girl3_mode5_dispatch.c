/* @SymbolSize: 136 */
/* @PostProc: patch_stack_complete --old 216 --new 232 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x80152BB4 */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[47];
} JumpTable80152BB4;

extern JumpTable80152BB4 date_girl3_mode5_dispatch_tbl;
extern u8 now_sub_sub_step;

void date_girl3_mode5_dispatch(void)
{
    JumpTable80152BB4 table;
    s32 idx;

    table = date_girl3_mode5_dispatch_tbl;
    idx = (s32)now_sub_sub_step;
    table.ptrs[idx](128);
}
