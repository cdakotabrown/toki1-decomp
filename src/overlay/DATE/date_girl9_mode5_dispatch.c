/* @SymbolSize: 124 */
/* @PostProc: patch_stack_complete --old 192 --new 208 */
/* @PostProc: remap_stack_slots --from 32 --to 44 */
/* Address: 0x801584D4 */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[40];
} JumpTable801584D4;

extern JumpTable801584D4 date_girl9_main5_tbl;
extern u8 now_sub_sub_step;

void date_girl9_mode5_dispatch(void)
{
    JumpTable801584D4 table;
    s32 idx;

    table = date_girl9_main5_tbl;
    idx = (s32)now_sub_sub_step;
    table.ptrs[idx](128);
}
