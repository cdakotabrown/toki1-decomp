/* @PostProc: patch_stack --old 112 --new 128 */
/* @PostProc: remap_stack_slots --from 32 --to 44 */
/* Address: 0x8013808C */
/* Size: 204 bytes (51 instructions) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[20];
} JumpTable;

extern void device_cmd_send(s32);

extern u8 now_sub_sub_step;
extern s32 step_counter;
extern s16 shougatu_date_now_serifu;
extern s16 shougatu_date_now_serifu_in;
extern JumpTable shougatu_step_dispatch3_tbl;

void shougatu_step_dispatch3(void)
{
    JumpTable table;
    s32 v;
    u32 cond;

    table = shougatu_step_dispatch3_tbl;

    table.ptrs[now_sub_sub_step]();

    if (shougatu_date_now_serifu != 1) goto done;
    if (shougatu_date_now_serifu_in != 8) goto done;
    v = step_counter;
    cond = ((u32)v < 1U);
    step_counter = v + 1;
    if (cond == 0) goto done;
    device_cmd_send(1280);
done:
    return;
}
