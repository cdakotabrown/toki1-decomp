/* @SymbolSize: 116 */
/* @PostProc: patch_stack_complete --old 256 --new 272 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x80153E08 */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[57];
} JumpTable80153E08;

extern JumpTable80153E08 date_girl7_mode4_dispatch_tbl;
extern u8 now_sub_sub_step;

void date_girl7_mode4_dispatch(void)
{
    JumpTable80153E08 table;
    s32 idx;

    table = date_girl7_mode4_dispatch_tbl;
    idx = (s32)now_sub_sub_step;
    table.ptrs[idx](128);
}
