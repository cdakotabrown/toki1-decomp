/* @PostProc: patch_stack_complete --old 112 --new 136 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* @SymbolSize: 124 */
/* Address: 0x80132214 */
/* Size: 124 bytes (31 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[22];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable kangei_sub_dispatch_tbl;

void kangei_sub_dispatch(void)
{
    JumpTable table;

    table = kangei_sub_dispatch_tbl;

    table.ptrs[now_sub_sub_step](128);
}
