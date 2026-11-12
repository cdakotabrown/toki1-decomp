/* @SymbolSize: 164 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* @PostProc: patch_stack_complete --old 80 --new 104 */
/* ending_sub_step_switch - ENDING overlay, Address: 0x801399D4, Size: 164 bytes (41 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[14];
} JumpTable;

extern JumpTable ending_sub_step_switch_tbl;
extern u8 now_sub_sub_step;
extern s32 event_girl_no;
extern void ending_fade_disp(s32 arg);

void ending_sub_step_switch(void)
{
    JumpTable table;

    table = ending_sub_step_switch_tbl;

    table.ptrs[now_sub_sub_step](128);

    if (event_girl_no != 0) {
        ending_fade_disp(0x404040);
    }
}
