/* @SymbolSize: 136 */
/* @PostProc: patch_stack_complete --old 184 --new 200 */
/* @PostProc: remap_stack_slots --from 32 --to 44 */
/* Address: 0x80155D68 */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[38];
} JumpTable80155D68;

extern JumpTable80155D68 date_girl5_main3_tbl;
extern u8 now_sub_sub_step;

void date_girl5_main3(void)
{
    JumpTable80155D68 table;
    s32 idx;

    table = date_girl5_main3_tbl;
    idx = (s32)now_sub_sub_step;
    table.ptrs[idx](128);
}
