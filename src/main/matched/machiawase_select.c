/* Function: machiawase_select */
/* Address: 0x80076BDC (the recorded symbol; the real entry is 0x80076BD4) */
/* Size: 300 bytes (75 instructions) recorded; 308 bytes (77 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @SymbolStart: 0x80076BD4 */
/* @SymbolSize: 308 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 IDO52_CONST_NOINREG_VALUES=2 IDO52_AS1_XBB_VOLATILE_OK=6 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 xa_wait(void);
extern void k_sub_reset(void);
extern void machiawase_menu_set(void);
extern s32 k_sub_disp_start(s32);
extern void menu_check(s32, s16, s16);
extern void menu_cursor_move(s32);
extern s32 menu_bar_show(s32);
extern s32 sub_sub_step_up(void);

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern volatile u8 step_mode;
extern u32 pad_trig;
extern volatile s8 menu_csr;
extern u8 machiawase_mode;
extern u8 machiawase_basyo[];
extern u8 machiawase_basyo3;

s32 machiawase_select(void) {
    s32 mode;
    s32 ret;

    mode = step_mode;
    ret = mode;
    switch (mode) {
    case 0:
        ret = xa_wait();
        break;
    case 1:
        k_sub_reset();
        machiawase_menu_set();
        mode = step_mode;
        step_mode = mode + 1;
        ret = k_sub_disp_start(3);
        break;
    case 2:
        menu_check(1, cursor_cw_x, cursor_cw_y);
        menu_cursor_move(1);
        ret = menu_bar_show(1);
        if (pad_trig & 0x20) {
            ret = menu_csr;
            switch (ret) {
            case 0:
            case 1:
                machiawase_mode = machiawase_basyo[menu_csr];
                ret = sub_sub_step_up();
                break;
            case 2:
                if (machiawase_mode == 2) {
                    machiawase_mode = machiawase_basyo3;
                    ret = sub_sub_step_up();
                }
                break;
            }
        }
        break;
    }
    return ret;
}
