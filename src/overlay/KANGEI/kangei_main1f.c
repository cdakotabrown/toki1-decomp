/* @PostProc: patch_stack_complete --old 184 --new 208 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* @SymbolSize: 124 */
/* Address: 0x80136018 */
/* Size: 124 bytes (31 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[40];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable kangei_main1f_tbl;

void kangei_main1f(void)
{
    JumpTable table;

    table = kangei_main1f_tbl;

    table.ptrs[now_sub_sub_step](128);
}
