/* @SymbolSize: 124 */
/* @PostProc: patch_stack_complete --old 200 --new 216 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x8014EB60 */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[43];
} JumpTable8014EB60;

extern JumpTable8014EB60 date_main5_tbl;
extern u8 now_sub_sub_step;

void date_girl8_mode5_step(void)
{
    JumpTable8014EB60 table;
    s32 idx;

    table = date_main5_tbl;
    idx = (s32)now_sub_sub_step;
    table.ptrs[idx](128);
}
