/* @SymbolStart: 0x8014A9F4 */
/* @SymbolSize: 232 */
/* @CompileEnv: IDO52_COPYPROP_VAR=-12 */
/* Address: 0x8014A9F4 (cross-prologue head: lbu v1, step_mode) / */
#include "types.h"

extern void goto_sub_step(s32);
extern void k_sub_reset(void);
extern void gnsx(s32);
extern void sndisp(s32, s32, s32);
extern void xa_wait(void);
extern s32 get_etc_sel_index(void);

extern u8 p_sei;
extern u8 sinro;
extern u16 bukatu_rec[][2];
extern u8 step_mode;
extern s32 etc_sound0_msg_tbl[];

s32 etc_sound_main0(void)
{
    u32 v1 = step_mode;
    s32 idx;

    switch (v1) {
    case 0:
        k_sub_reset();
        gnsx((s32)&p_sei);
        idx = get_etc_sel_index();
        sndisp(etc_sound0_msg_tbl[idx], 0, 15);
        if ((u32)bukatu_rec[idx][0] >= 411) {
            sinro = 64;
        } else {
            sinro = 192;
        }
        step_mode = step_mode + 1;
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        goto_sub_step(4);
        break;
    }
}
