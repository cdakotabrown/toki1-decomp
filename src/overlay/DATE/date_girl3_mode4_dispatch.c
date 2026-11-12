/* @SymbolSize: 116 */
/* @PostProc: patch_stack_complete --old 200 --new 216 */
/* @PostProc: remap_stack_slots --from 32 --to 44 */
/* Address: 0x80152B08 */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[42];
} JumpTable80152B08;

extern JumpTable80152B08 date_girl3_mode4_dispatch_tbl;
extern u8 now_sub_sub_step;

void date_girl3_mode4_dispatch(void)
{
    JumpTable80152B08 table;
    s32 idx;

    table = date_girl3_mode4_dispatch_tbl;
    idx = (s32)now_sub_sub_step;
    table.ptrs[idx](128);
}
