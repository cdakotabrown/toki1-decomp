/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: patch_stack_complete --old 48 --new 56 */
/* @SymbolStart: 0x8013A42C */
/* @RodataBase: 0x8013FFD4 */
/* @SymbolSize: 624 */
/* tel_tatebayasi_dialog - TEL overlay, Address: 0x8013A43C, Size: 608 bytes (152 insns) */
#include "types.h"

extern u8 step_mode;
extern u8 girl11_tel_cnt;
extern s32 *tel_tatebayasi_serifu_tbl[];
extern s32 tel_tatebayasi_xa_tbl[];
extern s32 tel_tatebayasi_xa_tbl2[];
extern u8 p_sei;
extern u32 date_name_set_tbl0[];

extern void xa_wait(void);
extern void k_sub_reset(void);
extern void gnsx(s32);
extern void sndisp(s32, s32, s32);
extern void xa_set_m(s32, s32, s32, s32);
extern void k_sub_disp_start(s32);
extern void xa_wait_m(void);
extern void sub_sub_step_up(void);

void tel_tatebayasi_dialog(void)
{
    s32 charIdx;

    charIdx = girl11_tel_cnt;

    switch (step_mode) {
        case 0:
            xa_wait();
            return;
        case 1:
            k_sub_reset();
            gnsx((*(s32 *)&date_name_set_tbl0[11]));
            sndisp(tel_tatebayasi_serifu_tbl[charIdx][0], 0, 11);
            xa_set_m(tel_tatebayasi_xa_tbl[charIdx], tel_tatebayasi_xa_tbl2[charIdx], 0, 11);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            return;
        case 2:
            xa_wait_m();
            return;
        case 3:
            k_sub_reset();
            gnsx((s32)&p_sei);
            sndisp(tel_tatebayasi_serifu_tbl[charIdx][1], 0, 15);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            return;
        case 4:
            xa_wait();
            return;
        case 5:
            k_sub_reset();
            gnsx((*(s32 *)&date_name_set_tbl0[11]));
            sndisp(tel_tatebayasi_serifu_tbl[charIdx][2], 0, 11);
            xa_set_m(tel_tatebayasi_xa_tbl[charIdx], tel_tatebayasi_xa_tbl2[charIdx], 2, 11);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            return;
        case 6:
            xa_wait_m();
            return;
        case 7:
            k_sub_reset();
            gnsx((s32)&p_sei);
            sndisp(tel_tatebayasi_serifu_tbl[charIdx][3], 0, 15);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            return;
        case 8:
            xa_wait();
            return;
        default:
            sub_sub_step_up();
            return;
    }
}
