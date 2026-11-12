/* @SymbolSize: 116 */
/* @PostProc: patch_stack_complete --old 176 --new 192 */
/* @PostProc: remap_stack_slots --from 32 --to 44 */
/* Address: 0x80158588 */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[36];
} JumpTable80158588;

extern JumpTable80158588 date_girl9_main6_tbl;
extern u8 now_sub_sub_step;

void date_girl9_mode6_dispatch(void)
{
    JumpTable80158588 table;
    s32 idx;

    table = date_girl9_main6_tbl;
    idx = (s32)now_sub_sub_step;
    table.ptrs[idx](128);
}
