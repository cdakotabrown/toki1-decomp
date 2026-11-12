/* @SymbolSize: 180 */
/* omimai_main0 - OMIMAI overlay, Address: 0x801322E4, Size: 200 bytes (50 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[25];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable omimai_main0_tbl;
extern void update_anim_state(s32);
extern void parameter_show(void);

void omimai_main0(void)
{
    u8 idx;
    JumpTable localTable;

    localTable = omimai_main0_tbl;

    idx = now_sub_sub_step;
    localTable.ptrs[idx](128);

    idx = now_sub_sub_step;
    if ((u32)idx < 14) {
        update_anim_state(1);
        parameter_show();
    } else {
        update_anim_state(2);
    }
}
