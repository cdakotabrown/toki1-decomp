/* geko_sub_sub_dispatch58 -- GEKO overlay, Address: 0x8013BB0C, Size: 148 bytes (37 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[58];
} JumpTable;

extern void device_cmd_send(s32 a0);

extern u8 now_sub_sub_step;
extern JumpTable geko_sub_sub_dispatch58_tbl;

void geko_sub_sub_dispatch58(void)
{
    s32 pad_hi;
    JumpTable table;

    table = geko_sub_sub_dispatch58_tbl;

    if (now_sub_sub_step == 27) {
        device_cmd_send(207);
    }
    table.ptrs[now_sub_sub_step](128);
}
