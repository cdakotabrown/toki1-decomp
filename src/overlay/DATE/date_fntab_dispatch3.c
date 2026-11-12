/* @SymbolSize: 136 */
/* Address: 0x8014FAD0 */
/* @PostProc: patch_stack_complete --old 192 --new 208 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[41];
} JumpTable8014FAD0;

extern JumpTable8014FAD0 date_fntab_dispatch3_tbl;
extern u8 now_sub_sub_step;

void date_fntab_dispatch3(void)
{
    JumpTable8014FAD0 table;
    s32 idx;

    table = date_fntab_dispatch3_tbl;
    idx = (s32)now_sub_sub_step;
    table.ptrs[idx](128);
}
