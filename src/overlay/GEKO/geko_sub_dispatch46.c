/* @PostProc: patch_stack_complete --old 208 --new 232 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x8013E03C */
/* Size: 132 bytes (33 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[46];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable geko_sub_dispatch46_tbl;
extern void geko_anim(void);

void geko_sub_dispatch46(void)
{
    JumpTable table;

    table = geko_sub_dispatch46_tbl;

    table.ptrs[now_sub_sub_step](128);
    geko_anim();
}
