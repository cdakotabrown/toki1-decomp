/* @PostProc: patch_stack_complete --old 304 --new 328 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* @SymbolSize: 124 */
/* Address: 0x80132274 */
/* Size: 124 bytes (31 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[70];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable shugaku_main6_tbl;

void shugaku_dispatch6(void)
{
    JumpTable table;

    table = shugaku_main6_tbl;

    table.ptrs[now_sub_sub_step](128);
}
