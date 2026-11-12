/* date2_answer_react_step - DATE2 overlay, Address: 0x801375D4, Size: 136 bytes (34 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[23];
} DispatchTable23;

extern DispatchTable23 date2_main3_tbl;
extern u8 now_sub_sub_step;

void date2_answer_react_step(void)
{
    u8 idx;
    DispatchTable23 table;

    table = date2_main3_tbl;
    idx = now_sub_sub_step;
    table.ptrs[idx](128);
}
