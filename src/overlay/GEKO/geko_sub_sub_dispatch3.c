/* @PostProc: patch_stack_complete --old 120 --new 136 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x8013D64C */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[23];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable geko_sub_sub_dispatch3_tbl;

void geko_sub_sub_dispatch3(void)
{
    JumpTable table;

    table = geko_sub_sub_dispatch3_tbl;

    table.ptrs[now_sub_sub_step]();
}
