/* date2_intro_step - DATE2 overlay, Address: 0x801326F0, Size: 172 bytes (43 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[31];
} DT31;

extern DT31 date2_main0_tbl;
extern u8 now_sub_sub_step;
extern u8 now_sub_step;

void update_anim_state(s32 arg0);

void date2_intro_step(void)
{
    u8 idx;
    DT31 table;

    table = date2_main0_tbl;
    idx = now_sub_sub_step;
    table.ptrs[idx](128);
    if (now_sub_step == 0) {
        if (now_sub_sub_step < 19U) {
            update_anim_state(1);
        }
    }
}
