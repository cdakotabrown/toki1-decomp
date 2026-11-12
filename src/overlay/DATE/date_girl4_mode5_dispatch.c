/* @SymbolSize: 116 */
/* @PostProc: patch_stack_complete --old 232 --new 248 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x80159340 */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[51];
} JumpTable80159340;

extern JumpTable80159340 date_girl4_main5_tbl;
extern u8 now_sub_sub_step;

void date_girl4_mode5_dispatch(void)
{
    JumpTable80159340 table;
    s32 idx;

    table = date_girl4_main5_tbl;
    idx = (s32)now_sub_sub_step;
    table.ptrs[idx](128);
}
