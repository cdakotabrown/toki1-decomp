/* Address: 0x80150930 */
/* Size: 116 bytes (29 instructions) */
/* @PostProc: patch_stack_complete --old 688 --new 704 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[165];
} JumpTable;

extern u8 now_sub_step;
extern JumpTable bunkasai_sub_dispatch_tbl;

void bunkasai_sub_dispatch(void)
{
    JumpTable table;

    table = bunkasai_sub_dispatch_tbl;

    table.ptrs[now_sub_step]();
}
