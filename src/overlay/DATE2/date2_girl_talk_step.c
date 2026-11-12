/* @SymbolSize: 124 */
/* date2_girl_talk_step -- DATE2 overlay, Address: 0x80132DE0, Size: 148 bytes (37 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[58];
} DispatchTable58;

extern DispatchTable58 date2_main1_tbl;
extern u8 now_sub_sub_step;

void date2_girl_talk_step(void)
{
    u8 idx;
    DispatchTable58 table;
    s32 pad;

    table = date2_main1_tbl;
    idx = now_sub_sub_step;
    table.ptrs[idx](128);
}
