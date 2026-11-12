/* @SymbolStart: 0x8014B724 */
/* @SymbolSize: 296 */
/* @RodataBase: 0x8014DB38 */
#include "types.h"

extern void k_sub_reset(void);
extern void gnsx(void *);
extern void sndisp(void *, s32, s32);
extern void xa_wait(void);
extern void x_taku_menu_set(s32, s32);
extern void x_taku_string_set(void *, void *, void *, s32);
extern void k_sub_disp_start(s32);
extern void sub_sub_step_up(void);

extern u8 p_sei[];
extern u8 etc_soudan_taku_step_str0[];
extern u8 etc_soudan_taku_step_str1[];
extern u8 etc_singaku_ans_str[];
extern u8 etc_syuusyoku_ans_str[];
extern u8 etc_soudan_taku_step_str2[];
extern u8 step_mode;
extern u8 now_icon;

void etc_soudan_taku_step(void)
{
    switch (step_mode) {
    case 0:
        k_sub_reset();
        gnsx(p_sei);
        sndisp(etc_soudan_taku_step_str0, 0, 15);
        step_mode++;
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        k_sub_reset();
        gnsx(p_sei);
        sndisp(etc_soudan_taku_step_str1, 0, 31);
        step_mode++;

    case 3:
        xa_wait();
        break;
    case 4:
        k_sub_reset();
        x_taku_menu_set(0, 3);
        x_taku_string_set(etc_singaku_ans_str, etc_syuusyoku_ans_str, etc_soudan_taku_step_str2, 3);
        k_sub_disp_start(3);
        now_icon = 0;
        sub_sub_step_up();
        break;
    }
}
