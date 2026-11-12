/* @SymbolSize: 136 */
/* @PostProc: patch_stack_complete --old 288 --new 304 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x8014FC70 */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[65];
} JumpTable8014FC70;

extern JumpTable8014FC70 date_fntab_dispatch5_tbl;
extern u8 now_sub_sub_step;

void date_fntab_dispatch5(void)
{
    JumpTable8014FC70 table;
    s32 idx;

    table = date_fntab_dispatch5_tbl;
    idx = (s32)now_sub_sub_step;
    table.ptrs[idx]();
}
