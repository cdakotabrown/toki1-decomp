/* @SymbolSize: 136 */
/* @PostProc: patch_stack_complete --old 192 --new 208 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x8015749C */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[41];
} JumpTable8015749C;

extern JumpTable8015749C date_girl6_main4_tbl;
extern u8 now_sub_sub_step;

void date_girl6_mode4_dispatch(void)
{
    JumpTable8015749C table;
    s32 idx;

    table = date_girl6_main4_tbl;
    idx = (s32)now_sub_sub_step;
    table.ptrs[idx](128);
}
