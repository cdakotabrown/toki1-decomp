/* @SymbolStart: 0x8014BFEC */
/* @RodataBase: 0x8014DB70 */
/* Address: 0x8014BFF4 */
/* Size: 476 bytes (119 insns) */
/* @SymbolSize: 484 */
/* @PostProc: scheduler_normalizer --const-fp --store-fp --branch-fp */
#include "types.h"

extern void k_sub_reset(void);
extern void gnsx(s32);
extern void sndisp(s32, s32, s32);
extern void xa_wait(void);
extern s32  etc_bucket_get(void);
extern void xa_set_m(s32, s32, s32, s32);
extern void xa_wait_m(void);
extern void goto_sub_step(s32);

extern u8  p_sei[];
extern u8  sinro;
extern u8  step_mode;
extern u8  etc_syuusyoku_ans_str[];
extern u8  etc_sinro_tutae_str[];
extern u8  etc_syuusyoku_step_str0[];
extern u8  etc_syuusyoku_xa_tbl[];
extern u8  etc_syuusyoku_xa2_tbl[];
extern s32 etc_syuusyoku_msg_tbl[];
extern s32 etc_sinro_msg_tbl[];

void etc_syuusyoku_step(void) {
    s32 idx = step_mode;

    switch (idx) {
    case 0:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp((s32)etc_syuusyoku_ans_str, 0, 15);
        sinro = 1;
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
        gnsx((s32)etc_syuusyoku_step_str0);
        sndisp(etc_syuusyoku_msg_tbl[etc_bucket_get() & 3], 0, 14);
        xa_set_m((s32)etc_syuusyoku_xa_tbl, (s32)etc_syuusyoku_xa2_tbl, etc_bucket_get() & 3, 14);
        step_mode++;
        break;
    case 5:
        xa_wait_m();
        break;
    case 6:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp(etc_sinro_msg_tbl[etc_bucket_get() & 3], 0, 15);
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
