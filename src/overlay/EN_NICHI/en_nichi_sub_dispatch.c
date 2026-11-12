/* @SymbolSize: 100 */
/* @PostProc: patch_stack_complete --old 40 --new 56 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x80136C90 */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[3];
} JumpTable80136C90;

extern JumpTable80136C90 en_nichi_sub_dispatch_tbl;
extern u8 now_sub_step;

void en_nichi_sub_dispatch(void)
{
    JumpTable80136C90 table;
    s32 idx;

    table = en_nichi_sub_dispatch_tbl;
    idx = (s32)now_sub_step;
    table.ptrs[idx]();
}
