/* @PostProc: patch_stack_complete --old 272 --new 288 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x8013F7B0 */
/* Size: 220 bytes (55 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[61];
} JumpTable;

extern u8 now_sub_sub_step;
extern u8 geko_fill_on;
extern s32 geko_ptc_cnt;
extern JumpTable geko_sub_step_dispatch_tbl;

extern void geko_fill_disp(void);
extern void geko_particle_move2(void);

void geko_sub_step_dispatch(void)
{
    JumpTable table;

    table = geko_sub_step_dispatch_tbl;

    table.ptrs[now_sub_sub_step](128);

    if (geko_fill_on != 0) {
        if ((u32)now_sub_sub_step >= 32U) {
            geko_fill_disp();
        }
    }

    if (geko_ptc_cnt != 0) {
        if ((u32)now_sub_sub_step >= 32U) {
            geko_particle_move2();
        }
    }
}
