/* @PostProc: patch_stack_complete --old 200 --new 216 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x80141B2C */
/* Size: 152 bytes (38 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[43];
} JumpTable;

extern u8 now_sub_sub_step;
extern u8 geko_bustup_kuchi_flag;
extern JumpTable geko_bustup_dispatch_tbl;

extern void bustup_kuchi_ani(void);

void geko_bustup_dispatch(void)
{
    JumpTable table;

    table = geko_bustup_dispatch_tbl;

    table.ptrs[now_sub_sub_step](128);

    if (geko_bustup_kuchi_flag != 0) {
        bustup_kuchi_ani();
    }
}
