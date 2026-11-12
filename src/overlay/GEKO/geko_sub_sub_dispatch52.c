/* @PostProc: patch_stack_complete --old 232 --new 256 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x801394AC */
/* Size: 132 bytes (33 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[52];
} JumpTable;

extern u8 now_sub_sub_step;
extern u8 cal_cw_mode;
extern JumpTable geko_sub_sub_dispatch52_tbl;

void geko_sub_sub_dispatch52(void)
{
    JumpTable table;

    table = geko_sub_sub_dispatch52_tbl;

    table.ptrs[now_sub_sub_step](128);
    cal_cw_mode = 1;
}
