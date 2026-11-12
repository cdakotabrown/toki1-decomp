/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x8014B94C */
/* @SymbolSize: 192 */
/* Address: 0x8014B954, Size: 184 bytes (46 insns) */
#include "types.h"

extern u8 step_mode;
extern u8 now_icon;
extern u32 p_sei;
extern u32 etc_soudan_girl_taku_step_str0;
extern s32 sub_sub_step_up(void);
extern void k_disp_start(s32);
extern void k_sub_reset(void);
extern void gnsx(void *);
extern s32 sndisp(void *, s32, s32);
extern void menu_girl_taku_set(s32, s32 *);
extern s32 xa_wait(void);

s32 etc_soudan_girl_taku_step(void)
{
    s32 ret;
    s32 buf;

    ret = step_mode;
    switch (step_mode) {
    case 0:
        k_sub_reset();
        gnsx(&p_sei);
        ret = sndisp(&etc_soudan_girl_taku_step_str0, 0, 15);
        step_mode = step_mode + 1;
        break;
    case 1:
        ret = xa_wait();
        break;
    case 2:
        k_sub_reset();
        menu_girl_taku_set(0, &buf);
        k_disp_start(buf);
        now_icon = 0;
        ret = sub_sub_step_up();
        break;
    }
    return ret;
}
