/* @SymbolSize: 136 */
/* @PostProc: patch_stack_complete --old 208 --new 224 */
/* @PostProc: remap_stack_slots --from 32 --to 44 */
/* Address: 0x8015A1B8 */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[44];
} JumpTable8015A1B8;

extern JumpTable8015A1B8 date_girl10_main5_tbl;
extern u8 now_sub_sub_step;

void date_girl10_mode5_dispatch(void)
{
    JumpTable8015A1B8 table;
    s32 idx;

    table = date_girl10_main5_tbl;
    idx = (s32)now_sub_sub_step;
    table.ptrs[idx](128);
}
