/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:7 */
/* @SymbolStart: 0x8014954C */
/* @SymbolSize: 464 */
/* Address: 0x80149554 */
/* Size: 456 bytes (114 instructions) */
/* @PostProc: remap_stack_slots --map '44:40' */
#include "types.h"

extern void k_sub_reset(void);
extern void gnsx(s32);
extern void sndisp(s32, s32, s32);
extern void xa_wait(void);
extern s32  get_etc_sel_index(void);
extern s32  x_taku_menu_set(s32, s32);
extern void x_taku_string_set(s32, s32, s32, s32);
extern void k_sub_disp_start(s32);
extern void sub_sub_step_up(void);

extern u8  p_sei[];
extern u8  sinro;
extern u8  step_mode;
extern s8  now_icon;
extern s32 etc_taku_str_tbl[];
extern u8  etc_taku_step_str1[];
extern u8  etc_taku_step_str4[];
extern u8  etc_taku_step_str5[];
extern u8  etc_taku_step_str0[];
extern u8  etc_taku_step_str2[];
extern u8  etc_taku_step_str3[];

s32 etc_taku_step(void) {
    s32 sel;
    s32 v1;
    u8 r;

    r = step_mode;
    v1 = r;
    switch (v1) {
    default:
        return r;
    case 0:
        k_sub_reset();
        gnsx((s32)&p_sei);
        if (sinro != 0) {
            sndisp((s32)&etc_taku_step_str0, 0, 15);
        } else {
            sndisp((s32)&etc_taku_step_str1, 0, 15);
        }
        step_mode++;
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        k_sub_reset();
        sel = get_etc_sel_index();
        if (sel != -1) {
            x_taku_menu_set(0, 3);
            if (sinro != 0) {
                x_taku_string_set((s32)&etc_taku_step_str2, (s32)&etc_taku_step_str3,
                              etc_taku_str_tbl[sel], 3);
            } else {
                x_taku_string_set((s32)&etc_taku_step_str4, (s32)&etc_taku_step_str5,
                              etc_taku_str_tbl[sel], 3);
            }
            k_sub_disp_start(3);
            now_icon = 0;
        } else {
            x_taku_menu_set(0, 2);
            if (sinro != 0) {
                x_taku_string_set((s32)&etc_taku_step_str2, (s32)&etc_taku_step_str3, 0, 2);
            } else {
                x_taku_string_set((s32)&etc_taku_step_str4, (s32)&etc_taku_step_str5, 0, 2);
            }
            k_sub_disp_start(2);
            now_icon = 0;
        }
        sub_sub_step_up();
        break;
    }
}
