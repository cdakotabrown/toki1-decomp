/* @SymbolSize: 136 */
/* omimai_sub_sub_dispatch - OMIMAI overlay, Address: 0x80133424, Size: 144 bytes (36 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[26];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable omimai_sub_sub_dispatch_tbl;

void omimai_sub_sub_dispatch(void)
{
    u8 idx;
    JumpTable localTable;

    localTable = omimai_sub_sub_dispatch_tbl;
    idx = now_sub_sub_step;
    localTable.ptrs[idx](128);
}
