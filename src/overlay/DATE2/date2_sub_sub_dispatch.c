/* date2_sub_sub_dispatch - DATE2 overlay, Address: 0x80136660, Size: 136 bytes (34 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[44];
} DispatchTable44;

extern DispatchTable44 date2_sub_sub_dispatch_tbl;
extern u8 now_sub_sub_step;

void date2_sub_sub_dispatch(void)
{
    u8 idx;
    DispatchTable44 table;

    table = date2_sub_sub_dispatch_tbl;
    idx = now_sub_sub_step;
    table.ptrs[idx](128);
}
