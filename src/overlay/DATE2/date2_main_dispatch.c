/* date2_main_dispatch - DATE2 overlay, Address: 0x80138048, Size: 124 bytes (31 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[13];
} DispatchTable13;

extern DispatchTable13 date2_main_dispatch_tbl;
extern u8 now_sub_sub_step;

void date2_main_dispatch(void)
{
    u8 idx;
    DispatchTable13 table;

    table = date2_main_dispatch_tbl;
    idx = now_sub_sub_step;
    table.ptrs[idx](64);
}
