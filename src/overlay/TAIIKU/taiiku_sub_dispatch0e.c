/* @SymbolSize: 124 */
/* @PostProc: patch_stack_complete --old 72 --new 88 */
/* @PostProc: remap_stack_slots --from 32 --to 44 */
/* Address: 0x80142414 */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[10];
} JumpTable80142414;

extern JumpTable80142414 taiiku_ninin_step_tbl;
extern u8 now_sub_step;

void taiiku_sub_dispatch0e(void)
{
    JumpTable80142414 table;
    s32 idx;

    table = taiiku_ninin_step_tbl;
    idx = (s32)now_sub_step;
    table.ptrs[idx]();
}
