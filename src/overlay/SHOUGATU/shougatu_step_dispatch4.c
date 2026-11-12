/* @PostProc: patch_stack_complete --old 104 --new 128 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x801381B8 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[20];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable shougatu_step_dispatch4_tbl;

void shougatu_step_dispatch4(void)
{
    JumpTable table;

    table = shougatu_step_dispatch4_tbl;

    table.ptrs[now_sub_sub_step]();
}
