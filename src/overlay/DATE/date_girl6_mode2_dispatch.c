/* @SymbolSize: 136 */
/* @PostProc: patch_stack_complete --old 232 --new 248 */
/* @PostProc: remap_stack_slots --from 32 --to 44 */
/* Address: 0x80156FCC */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[50];
} JumpTable80156FCC;

extern JumpTable80156FCC date_girl6_main2_tbl;
extern u8 now_sub_sub_step;

void date_girl6_mode2_dispatch(void)
{
    JumpTable80156FCC table;
    s32 idx;

    table = date_girl6_main2_tbl;
    idx = (s32)now_sub_sub_step;
    table.ptrs[idx](128);
}
