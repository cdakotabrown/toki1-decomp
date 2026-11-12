/* @SymbolStart: 0x8014A724 */
/* @RodataBase: 0x8014D948 */
/* @SymbolSize: 260 */
/* etc_siken_main2_0 - ETC overlay, Address: 0x8014A72C, Size: 252 bytes (63 insns) */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 k_sub_reset();
extern s32 k_sub_disp_start();
extern s32 x_taku_menu_set();
extern s32 x_taku_string_set();
extern s32 gnsx();
extern s32 sndisp();
extern s32 xa_wait();
extern char p_sei[];
extern u8 step_mode;
extern u8 sinro;
extern u8 now_icon;

extern char etc_siken_main2_0_str0[];
extern char etc_siken_main2_0_str1[];
extern char etc_siken_main2_0_str2[];
extern char etc_siken_main2_0_str3[];

void etc_siken_main2_0(void)
{
    switch (step_mode) {
    case 0:
        sinro = 1;
        k_sub_reset();
        gnsx(p_sei);
        sndisp(etc_siken_main2_0_str0, 0, 31);
        step_mode = step_mode + 1;
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        k_sub_reset();
        x_taku_menu_set(0, 3);
        x_taku_string_set(etc_siken_main2_0_str1, etc_siken_main2_0_str2, etc_siken_main2_0_str3, 3);
        k_sub_disp_start(3);
        now_icon = 0;
        sub_sub_step_up();
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        sub_sub_step_up();
        break;
    default:
        break;
    }
}
