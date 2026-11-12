/* @PostProc: swap_branch_operands --offsets 0x54 */
/* @SymbolSize: 220 */
#include "types.h"

extern u8 girl_no;
extern u8 ret_sub_step;
extern u8 shugaku_place;
extern u8 fade_bri;

extern s32  get_g_zyotai_s(s32 a0);
extern void goto_sub_step(s32 a0);
extern void sub_sub_step_up(void);

void shugaku_event_judge(void)
{
    s32 key;
    u8 id;

    id = girl_no;
    key = get_g_zyotai_s(id);
    if (ret_sub_step == 5) {
        id = girl_no;
        if (id == 2 && shugaku_place == 2) {
            fade_bri = 0;
            goto_sub_step(8);
            return;
        }
    }
    if (ret_sub_step == 3) {
        id = girl_no;
        if (id == 5 && (u8)(key & 0x7F) < 2U) {
            goto_sub_step(11);
            return;
        }
    }
    if (ret_sub_step == 3) {
        id = girl_no;
        if (id == 4 && (u8)(key & 0x7F) < 2U) {
            goto_sub_step(10);
            return;
        }
    }
    sub_sub_step_up();
}
