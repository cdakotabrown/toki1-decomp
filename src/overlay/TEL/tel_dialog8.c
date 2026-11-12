/* @SymbolStart: 0x80135798 */
/* @RodataBase: 0x8013F56C */
/* Address: 0x801357A0 */
/* Size: 464 bytes (116 insns) */
/* @SymbolSize: 472 */
/* @PostProc: scheduler_normalizer --const-fp --store-fp --branch-fp */
#include "types.h"

extern void k_sub_reset(void);
extern void gnsx(s32);
extern void sndisp(s32, s32, s32);
extern void xa_set_m(s32, s32, s32, s32);
extern void k_sub_disp_start(s32);
extern void xa_wait_m(void);
extern void xa_wait(void);
extern void goto_sub_sub_step(s32);
extern void menu_bar_show(s32);
extern void cal_base_show(void);

extern u8  tel_serifu_girl;
extern s32 date_name_set_tbl0[];
extern u8  p_sei[];
extern u8  step_mode;
extern s32 tel_fujisaki_odoroki_serifu_tbl;
extern u8  tel_fujisaki_odoroki_xa_tbl[];
extern s32 tel_fujisaki_odoroki_serifu_tbl_1;
extern s32 tel_goyou0_serifu_tbl[];
extern u8  tel_goyou0_xa_tbl[];
extern u8  tel_goyou0_xa_tbl2[];

void tel_dialog8(void) {
    s32 idx = step_mode;
    switch (idx) {
    case 0:
        k_sub_reset();
        gnsx(date_name_set_tbl0[tel_serifu_girl]);
        sndisp(tel_fujisaki_odoroki_serifu_tbl, 0, tel_serifu_girl);
        xa_set_m((s32)tel_fujisaki_odoroki_xa_tbl, (s32)tel_fujisaki_odoroki_xa_tbl, 0, tel_serifu_girl);
        k_sub_disp_start(0);
        step_mode++;
        break;
    case 1:
        xa_wait_m();
        break;
    case 2:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp(tel_fujisaki_odoroki_serifu_tbl_1, 0, 15);
        k_sub_disp_start(0);
        step_mode++;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        k_sub_reset();
        gnsx(date_name_set_tbl0[tel_serifu_girl]);
        sndisp(tel_goyou0_serifu_tbl[tel_serifu_girl], 0, tel_serifu_girl);
        xa_set_m((s32)tel_goyou0_xa_tbl, (s32)tel_goyou0_xa_tbl2, tel_serifu_girl, tel_serifu_girl);
        k_sub_disp_start(0);
        step_mode++;
        break;
    case 5:
        xa_wait_m();
        break;
    default:
        goto_sub_sub_step(32);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}
