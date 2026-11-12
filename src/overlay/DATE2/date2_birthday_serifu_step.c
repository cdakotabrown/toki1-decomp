/* date2_birthday_serifu_step - DATE2 overlay, Address: 0x80136FC4, Size: 160 bytes (40 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[8];
} DispatchTable8;

extern DispatchTable8 date2_main4_tbl;
extern u8 now_sub_sub_step;

void date2_birthday_serifu_step(void)
{
    u8 idx;
    DispatchTable8 table;

    table = date2_main4_tbl;
    idx = now_sub_sub_step;
    table.ptrs[idx](128);
}
