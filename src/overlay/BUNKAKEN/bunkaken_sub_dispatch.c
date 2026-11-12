/* @SymbolSize: 124 */
/* /* @PostProc: patch_stack_complete --old 592 --new 616 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x80145E80 */
/* Size: 124 bytes (31 instructions) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[142];
} JumpTable;

extern u8 now_sub_step;
extern JumpTable bunkaken_sub_dispatch_tbl;

void bunkaken_sub_dispatch(void)
{
    JumpTable table;

    table = bunkaken_sub_dispatch_tbl;

    table.ptrs[now_sub_step]();
}
