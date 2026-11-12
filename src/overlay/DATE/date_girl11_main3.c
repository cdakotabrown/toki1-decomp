/* @SymbolSize: 136 */
/* @PostProc: patch_stack_complete --old 112 --new 128 */
/* @PostProc: remap_stack_slots --from 32 --to 44 */
/* Address: 0x8015A498 */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[20];
} JumpTable8015A498;

extern JumpTable8015A498 date_girl11_main3_tbl;
extern u8 now_sub_sub_step;

void date_girl11_main3(void)
{
    JumpTable8015A498 table;
    s32 idx;

    table = date_girl11_main3_tbl;
    idx = (s32)now_sub_sub_step;
    table.ptrs[idx](128);
}
