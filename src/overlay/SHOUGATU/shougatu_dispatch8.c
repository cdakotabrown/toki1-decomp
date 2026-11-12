/* shougatu_dispatch8 - SHOUGATU overlay, Address: 0x80141698, Size: 136 bytes (34 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[11];
} DispatchTable11;

extern DispatchTable11 shougatu_dispatch8_tbl;
extern u8 now_sub_sub_step;

void shougatu_dispatch8(void)
{
    u8 idx;
    DispatchTable11 table;

    table = shougatu_dispatch8_tbl;
    idx = now_sub_sub_step;
    table.ptrs[idx](128);
}
