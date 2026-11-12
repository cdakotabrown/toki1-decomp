/* @SymbolSize: 136 */
/* @PostProc: patch_stack_complete --old 192 --new 208 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x80159280 */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[41];
} JumpTable80159280;

extern JumpTable80159280 date_girl4_main4_tbl;
extern u8 now_sub_sub_step;

void date_girl4_mode4_dispatch(void)
{
    JumpTable80159280 table;
    s32 idx;

    table = date_girl4_main4_tbl;
    idx = (s32)now_sub_sub_step;
    table.ptrs[idx](128);
}
