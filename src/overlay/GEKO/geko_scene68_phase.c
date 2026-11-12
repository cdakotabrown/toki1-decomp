/* @PostProc: patch_stack_complete --old 120 --new 136 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x8013F604 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[23];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable geko_scene68_phase_tbl;

void geko_scene68_phase(void)
{
    JumpTable table;

    table = geko_scene68_phase_tbl;

    table.ptrs[now_sub_sub_step](128);
}
