/* @PostProc: patch_stack_complete --old 168 --new 184 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x8013EECC */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[35];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable geko_girl4_27_phase_tbl;

void geko_girl4_27_phase(void)
{
    JumpTable table;

    table = geko_girl4_27_phase_tbl;

    table.ptrs[now_sub_sub_step]();
}
