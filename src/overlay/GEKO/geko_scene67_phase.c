/* @PostProc: patch_stack_complete --old 224 --new 240 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x8013F550 */
/* Size: 124 bytes (31 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[49];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable geko_scene67_phase_tbl;

void geko_scene67_phase(void)
{
    JumpTable table;

    table = geko_scene67_phase_tbl;

    table.ptrs[now_sub_sub_step](128);
}
