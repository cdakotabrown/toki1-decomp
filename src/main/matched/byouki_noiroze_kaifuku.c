/* Function: byouki_noiroze_kaifuku */
/* Address: 0x80073660 */
/* Size: 244 bytes (61 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @SymbolStart: 0x80073660 */
/* @SymbolSize: 244 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_AS1_TIE_REJECT=2 IDO52_AS1_XBB_SKIP_CAND=0,1 IDO52_AS1_TIE_TAKE=39 IDO52_AS1_FORCE_LA_PULL=0 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0x94:rt:t1:v1 0x98:rs:t1:v1 0x98:rt:t1:v1 0xa0:rs:t1:v1 0xa0:rt:t2:t1 0xa8:rt:t2:t1 */
/* @PostProc: patch_insn_reg_field 0xc8:rt:t3:t2 0xcc:rs:t3:t2 0xcc:rt:t3:t2 0xd4:rs:t3:t2 */
#include "types.h"

extern u8 step_mode;
extern s32 step_counter;
extern s32 pad_data;
extern volatile u8 byouki_kind;
extern volatile u8 byouki_state;
extern u8 p_sei[];

extern u8 byouki_noiroze_kaifuku_str0[];
extern u8 byouki_noiroze_kaifuku_str1[];

extern void text_sys_init(void);
extern void basyo_init2(u8 *);
extern void gnsx(u8 *);
extern void sndi(u8 *, s32, s32);
extern void k_disp_start(s32);
extern void goto_sub_step(s32);

s32 byouki_noiroze_kaifuku(void) {
    s32 state = step_mode;

    if (state == 0) {
        goto c0;
    }
    if (state == 1) {
        goto c1;
    }
    return state;
c0:
    text_sys_init();
    basyo_init2(byouki_noiroze_kaifuku_str0);
    byouki_state = (byouki_state & 0xFF1F) | 0x60;
    byouki_kind = byouki_kind & 0xFFF0;
    gnsx(p_sei);
    sndi(byouki_noiroze_kaifuku_str1, 0, 31);
    k_disp_start(3);
    step_mode++;
    return;
c1:
    if ((u32)step_counter < 61) {
        return;
    }
    if ((u32)step_counter < 257) {
        if (pad_data == 0) {
            return;
        }
    }
    goto_sub_step(255);
    return;
}
