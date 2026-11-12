/* @CompileEnv: IDO52_FORCE_WEB_COLOR=7:v1 */
/* @SymbolStart: 0x80135F28 */
/* @SymbolSize: 232 */
/* @PostProc: patch_insn_reg_field 0xa8:rt:t9:v1 0xac:rs:t9:v1 0xac:rt:t9:v1 0xb4:rs:t9:v1 0xb4:rt:t0:t9 0xbc:rt:t0:t9 */
#include "types.h"

extern u8 tel_serifu_girl;
extern u8 step_mode;
extern s32 date_name_set_tbl0[];
extern s32 tel_goyou0_serifu_tbl[];
extern u8 tel_goyou0_xa_tbl;
extern u8 tel_goyou0_xa_tbl2;

extern void k_sub_reset(void);
extern void gnsx(s32);
extern void sndisp(s32, s32, s32);
extern void xa_set_m(s32, s32, s32, s32);
extern void k_sub_disp_start(s32);
extern void xa_wait_m(void);
extern void goto_sub_sub_step(s32);
extern void menu_bar_show(s32);
extern void cal_base_show(void);

void tel_dialog2(void) {
    u8 arg;

    arg = step_mode;
    switch (arg) {
        case 0:
            k_sub_reset();
            gnsx(date_name_set_tbl0[tel_serifu_girl]);
            sndisp(tel_goyou0_serifu_tbl[tel_serifu_girl], 0, tel_serifu_girl);
            xa_set_m((s32)&tel_goyou0_xa_tbl, (s32)&tel_goyou0_xa_tbl2, tel_serifu_girl, tel_serifu_girl);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 1:
            xa_wait_m();
            break;
        default:
            goto_sub_sub_step(32);
            break;
    }
    menu_bar_show(0);
    cal_base_show();
}
