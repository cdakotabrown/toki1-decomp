/* @CompileFlags: -Wc,-pic1 */
/* @SymbolStart: 0x80135970 */
/* @RodataBase: 0x8013F584 */
/* @SymbolSize: 520 */
/* @PostProc: patch_stack_complete --old 56 --new 48 */
/* @PostProc: patch_insn_reg_field 0x6c:rt:a2:v1 0x70:rs:a2:v1 0x7c:rd:t0:t9 0x80:rd:t1:t9 0x80:rt:t0:t9 0x84:rt:t1:t9 0x90:rd:v1:t0 0x94:rs:v1:t0 0xa0:rd:v1:t1 0xa4:rd:t3:t2 0xa4:rt:v1:t1 0xa8:rt:t3:t2 0xb4:rt:v1:t1 0xe0:rt:t4:t3 0xe4:rs:t4:t3 0xe4:rt:t4:t3 0xec:rs:t4:t3 0xec:rt:t5:t4 0xf4:rt:t5:t4 0x138:rt:t6:t5 0x13c:rs:t6:t5 0x13c:rt:t6:t5 0x144:rs:t6:t5 0x144:rt:t7:t6 0x14c:rt:t7:t6 0x168:rt:t8:t7 0x16c:rs:t8:t7 0x16c:rt:t8:t7 0x174:rd:t9:t8 0x174:rt:t8:t7 0x178:rt:t9:t8 0x1c8:rt:t0:t9 0x1cc:rs:t0:t9 0x1cc:rt:t0:t9 0x1d4:rs:t0:t9 0x1d4:rt:t1:t0 0x1dc:rt:t1:t0 */
/* tel_dialog9 - TEL overlay, Address: 0x80135978, Size: 512 bytes (128 insns), Semi-matched */
#include "types.h"

extern u8 step_mode;
extern s32 date_name_set_tbl0[];
extern u8 girl0_tel_cnt[];
extern s32 p_sei;
extern s32 tel_kagami_kyoudai_serifu_tbl[];
extern s32 tel_kagami_kyoudai_xa_tbl[];
extern s32 tel_kagami_kyoudai_xa_tbl2[];
extern s32 tel_kagami_kyoudai_serifu_tbl_3;
extern s32 tel_kagami_kyoudai_serifu_tbl_4;
extern u8 tel_serifu_girl;

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

void tel_dialog9(void)
{
    s32 hidden_t6 = step_mode;
    u8 idx;
    u8 val;
    s32 anim_idx;

    switch (hidden_t6) {
        case 0:
            k_sub_reset();
            idx = tel_serifu_girl;
            gnsx(date_name_set_tbl0[idx]);
            idx = tel_serifu_girl;
            val = girl0_tel_cnt[(idx << 3) - idx << 3];
            anim_idx = (((u32)val >> 1)) % 3;
            sndisp(tel_kagami_kyoudai_serifu_tbl[anim_idx], 0, idx);
            idx = tel_serifu_girl;
            xa_set_m((s32)tel_kagami_kyoudai_xa_tbl, (s32)tel_kagami_kyoudai_xa_tbl2, anim_idx, idx);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 1:
            xa_wait_m();
            break;
        case 2:
            k_sub_reset();
            gnsx((s32)&p_sei);
            sndisp(tel_kagami_kyoudai_serifu_tbl_3, 0, 15);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 3:
            xa_wait();
            break;
        case 4:
            k_sub_reset();
            idx = tel_serifu_girl;
            gnsx(date_name_set_tbl0[idx]);
            idx = tel_serifu_girl;
            sndisp(tel_kagami_kyoudai_serifu_tbl_4, 0, idx);
            idx = tel_serifu_girl;
            xa_set_m((s32)tel_kagami_kyoudai_xa_tbl, (s32)tel_kagami_kyoudai_xa_tbl2, 4, idx);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
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
