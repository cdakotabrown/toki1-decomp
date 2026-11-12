/* @SymbolSize: 248 */
/* @PostProc: patch_stack_complete --old 304 --new 328 */
/* @PostProc: remap_stack_slots --from 156 --to 176 --from 24 --to 44 */
/* Function: kangei_switch2 */
/* Overlay: KANGEI */
#include "types.h"

typedef struct { u32 w[37]; } Tbl1;
typedef struct { u32 w[33]; } Tbl2;

extern Tbl1 kangei_switch2_tbl0;
extern Tbl2 kangei_switch2_tbl1;
extern u8 girl_no;
extern u8 now_sub_sub_step;
extern u8 kangei_switch2_done_flag;

void kangei_switch2(void)
{
    Tbl1 local1;
    Tbl2 local2;

    local1 = kangei_switch2_tbl0;
    local2 = kangei_switch2_tbl1;
    if ((u32)girl_no < 4U) {
        ((void (*)(s32))local1.w[now_sub_sub_step])(128);
        kangei_switch2_done_flag = 1;
    } else {
        ((void (*)(s32))local2.w[now_sub_sub_step])(128);
    }
}
