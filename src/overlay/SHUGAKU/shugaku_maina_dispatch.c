/* @SymbolSize: 124 */
/* shugaku_maina_dispatch - SHUGAKU overlay, Address: 0x80134960, Size: 124 bytes (31 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[25];
} DispatchTable25;

extern DispatchTable25 shugaku_maina_dispatch_tbl;
extern u8 now_sub_sub_step;

void shugaku_maina_dispatch(void)
{
    u8 idx;
    DispatchTable25 table;

    table = shugaku_maina_dispatch_tbl;
    idx = now_sub_sub_step;
    table.ptrs[idx](128);
}
