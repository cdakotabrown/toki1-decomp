/* @SymbolSize: 136 */
/* @PostProc: patch_stack_complete --old 216 --new 232 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x80158190 */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[47];
} JumpTable80158190;

extern JumpTable80158190 date_girl9_main3_tbl;
extern u8 now_sub_sub_step;

void date_girl9_mode3_dispatch(void)
{
    JumpTable80158190 table;
    s32 idx;

    table = date_girl9_main3_tbl;
    idx = (s32)now_sub_sub_step;
    table.ptrs[idx](128);
}
