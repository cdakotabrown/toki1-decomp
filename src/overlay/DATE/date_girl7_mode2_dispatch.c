/* @SymbolSize: 136 */
/* @PostProc: patch_stack_complete --old 192 --new 208 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x80153B24 */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[41];
} JumpTable80153B24;

extern JumpTable80153B24 date_girl7_mode2_dispatch_tbl;
extern u8 now_sub_sub_step;

void date_girl7_mode2_dispatch(void)
{
    JumpTable80153B24 table;
    s32 idx;

    table = date_girl7_mode2_dispatch_tbl;
    idx = (s32)now_sub_sub_step;
    table.ptrs[idx](128);
}
