/* @SymbolSize: 124 */
/* @PostProc: patch_stack_complete --old 296 --new 312 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x80154CB4 */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[67];
} JumpTable80154CB4;

extern JumpTable80154CB4 date_girl1_main2_tbl;
extern u8 now_sub_sub_step;

void date_girl1_mode2_dispatch(void)
{
    JumpTable80154CB4 table;
    s32 idx;

    table = date_girl1_main2_tbl;
    idx = (s32)now_sub_sub_step;
    table.ptrs[idx](128);
}
