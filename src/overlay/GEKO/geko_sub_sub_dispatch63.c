/* geko_sub_sub_dispatch63 -- GEKO overlay, Address: 0x801416C8, Size: 180 bytes (45 insns) */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* @PostProc: patch_stack_complete --old 280 --new 296 */
#include "types.h"

typedef s32 (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[63];
} JumpTable;

extern s32 date_light_effect(void);
extern s32 geko_light_disp(void);

extern u8 now_sub_sub_step;
extern JumpTable geko_sub_sub_dispatch63_tbl;

void geko_sub_sub_dispatch63(void)
{
    JumpTable table;
    u32 v;

    table = geko_sub_sub_dispatch63_tbl;

    table.ptrs[now_sub_sub_step](32);

    v = now_sub_sub_step;
    if (v >= 36 && v < 43) {
        date_light_effect();
        return;
    }
    if (v == 49) {
        geko_light_disp();
    }
}
