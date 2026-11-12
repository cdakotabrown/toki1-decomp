/* @SymbolSize: 112 */
/* @PostProc: patch_stack_complete --old 48 --new 64 */
/* @PostProc: remap_stack_slots --from 32 --to 44 */
/* Address: 0x80132140 */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[4];
} JumpTable80132140;

extern JumpTable80132140 master_sub_step_dispatch_tbl;
extern u8 now_sub_step;

void master_sub_step_dispatch(void)
{
    JumpTable80132140 table;
    s32 idx;

    table = master_sub_step_dispatch_tbl;
    idx = (s32)now_sub_step;
    table.ptrs[idx]();
}
