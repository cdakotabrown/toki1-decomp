/* @SymbolStart: 0x8014BB80 */
/* @RodataBase: 0x8014DB4C */
/* Address: 0x8014BB88 */
/* Size: 472 bytes (118 insns) */
/* @SymbolSize: 480 */
/* @PostProc: scheduler_normalizer --const-fp --store-fp --branch-fp */
#include "types.h"

extern void k_sub_reset(void);
extern void gnsx(s32);
extern void sndisp(s32, s32, s32);
extern void xa_wait(void);
extern s32  get_etc_gakuryoku(void);
extern void xa_set_m(s32, s32, s32, s32);
extern void xa_wait_m(void);
extern void goto_sub_step(s32);

extern u8  p_sei[];
extern u8  sinro;
extern u8  step_mode;
extern u8  etc_singaku_ans_str[];
extern u8  etc_sinro_tutae_str[];
extern u8  etc_singaku_step_str0[];
extern u8  etc_singaku_xa_tbl[];
extern u8  etc_singaku_xa2_tbl[];
extern s32 etc_singaku_msg_tbl[];
extern s32 etc_sinro_msg_tbl[];

void etc_singaku_step(void) {
    switch (step_mode) {
    case 0:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp((s32)etc_singaku_ans_str, 0, 15);
        sinro = 0;
        step_mode++;
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp((s32)etc_sinro_tutae_str, 0, 31);
        step_mode++;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        k_sub_reset();
        gnsx((s32)etc_singaku_step_str0);
        sndisp(etc_singaku_msg_tbl[get_etc_gakuryoku() & 3], 0, 14);
        xa_set_m((s32)etc_singaku_xa_tbl, (s32)etc_singaku_xa2_tbl, get_etc_gakuryoku() & 3, 14);
        step_mode++;
        break;
    case 5:
        xa_wait_m();
        break;
    case 6:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp(etc_sinro_msg_tbl[get_etc_gakuryoku() & 3], 0, 15);
        step_mode++;
        break;
    case 7:
        xa_wait();
        break;
    case 8:
        goto_sub_step(6);
        break;
    }
}
