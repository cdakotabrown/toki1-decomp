/* @SymbolSize: 124 */
/* @PostProc: patch_stack_complete --old 200 --new 216 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x801575AC */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[43];
} JumpTable801575AC;

extern JumpTable801575AC date_girl6_main5_tbl;
extern u8 now_sub_sub_step;

void date_girl6_mode5_dispatch(void)
{
    JumpTable801575AC table;
    s32 idx;

    table = date_girl6_main5_tbl;
    idx = (s32)now_sub_sub_step;
    table.ptrs[idx](128);
}
