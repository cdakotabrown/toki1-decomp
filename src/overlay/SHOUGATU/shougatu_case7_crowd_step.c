/* Address: 0x801425D0 */
/* Size: 124 bytes (31 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[51];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable shougatu_main7_tbl;

extern void shougatu_crowd(void);

void shougatu_case7_crowd_step(void)
{
    u8 idx;
    JumpTable table;

    table = shougatu_main7_tbl;

    idx = now_sub_sub_step;
    table.ptrs[idx](128);

    shougatu_crowd();
}
