/* shougatu_dispatch1 - SHOUGATU overlay, Address: 0x80141024, Size: 136 bytes (34 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[17];
} DispatchTable17;

extern DispatchTable17 shougatu_dispatch1_tbl;
extern u8 now_sub_sub_step;

void shougatu_dispatch1(void)
{
    u8 idx;
    DispatchTable17 table;

    table = shougatu_dispatch1_tbl;
    idx = now_sub_sub_step;
    table.ptrs[idx](128);
}
