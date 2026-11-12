/* @PostProc: patch_stack_complete --old 272 --new 296 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x80139204 */
/* Size: 176 bytes (44 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[62];
} JumpTable;

extern u8 now_sub_sub_step;
extern s16 shougatu_date_now_serifu;
extern s16 shougatu_date_now_serifu_in;
extern s32 bustup_on;
extern JumpTable shougatu_subsub_dispatch_tbl;

void shougatu_subsub_dispatch(void)
{
    JumpTable table;

    table = shougatu_subsub_dispatch_tbl;

    table.ptrs[now_sub_sub_step](128);

    if (shougatu_date_now_serifu == 0) {
        if (shougatu_date_now_serifu_in == 3) {
            bustup_on = 0;
        }
    }
}
