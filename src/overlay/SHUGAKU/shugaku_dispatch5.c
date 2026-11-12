/* @SymbolSize: 124 */
/* shugaku_dispatch5 - SHUGAKU overlay, Address: 0x8013AA10, Size: 124 bytes (31 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[25];
} DispatchTable25;

extern DispatchTable25 shugaku_dispatch5_tbl;
extern u8 now_sub_sub_step;

void shugaku_dispatch5(void)
{
    u8 idx;
    DispatchTable25 table;

    table = shugaku_dispatch5_tbl;
    idx = now_sub_sub_step;
    table.ptrs[idx](128);
}
