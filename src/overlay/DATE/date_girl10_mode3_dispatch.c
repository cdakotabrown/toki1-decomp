/* @SymbolSize: 136 */
/* @PostProc: patch_stack_complete --old 208 --new 224 */
/* @PostProc: remap_stack_slots --from 32 --to 44 */
/* Address: 0x80159EB4 */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[44];
} JumpTable80159EB4;

extern JumpTable80159EB4 date_girl10_main3_tbl;
extern u8 now_sub_sub_step;

void date_girl10_mode3_dispatch(void)
{
    JumpTable80159EB4 table;
    s32 idx;

    table = date_girl10_main3_tbl;
    idx = (s32)now_sub_sub_step;
    table.ptrs[idx](128);
}
