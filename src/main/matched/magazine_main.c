/* Function: magazine_main */
/* Address: 0x80077EBC */
/* Size: 252 bytes (63 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 step_mode;
extern u8 date2_menu_select_tbl1;
extern u32 *magazine_main_tbl0[4];
extern u8 p_sei[128];
extern s32 magazine_base_show(void);
extern void xa_wait(void);
extern void sub_sub_step_up(void);
extern void k_sub_reset(void);
extern void k_sub_disp_start(s32);
extern void gnsx(u8 *);
extern s32 magazine_slot_get(void);
extern void sndisp(u32 *, s32, s32);

void magazine_main(void) {
    s32 mode;

    mode = magazine_base_show();
    mode = step_mode;

    switch (mode) {
    case 0:
        xa_wait();
        break;
    case 1:
        if (date2_menu_select_tbl1 != 0) {
            step_mode++;
            break;
        }
        sub_sub_step_up();
        break;
    case 2:
        k_sub_reset();
        k_sub_disp_start(0);
        gnsx(p_sei);
        sndisp(magazine_main_tbl0[magazine_slot_get()], 0, 31);
        step_mode++;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
    default:
        sub_sub_step_up();
        break;
    }
}
