/* omimai_main1 - OMIMAI overlay, Address: 0x80132D44, Size: 124 bytes (31 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[12];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable omimai_main1_tbl;
extern void update_anim_state(s32);

void omimai_main1(void)
{
    u8 idx;
    JumpTable localTable;

    localTable = omimai_main1_tbl;
    idx = now_sub_sub_step;
    localTable.ptrs[idx](128);
    update_anim_state(2);
}
