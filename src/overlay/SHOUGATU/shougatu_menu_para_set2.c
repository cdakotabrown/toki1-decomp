/* @SymbolStart: 0x80140354 */
/* @SymbolSize: 144 */
/* shougatu_menu_para_set2 - SHOUGATU overlay, Address: 0x80140370, Size: 116 bytes (29 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void param_clamp(void);
extern s16 para_str;
extern u8  shougatu_menu_slot;
extern u8  shougatu_menu_girl_tbl[];
extern s16 shougatu_menu_now_serifu_in;
extern s16 shougatu_menu_now_serifu;

void shougatu_menu_para_set2(void)
{
    shougatu_menu_now_serifu_in = 0;
    if (shougatu_menu_girl_tbl[shougatu_menu_slot] == 11) {
        shougatu_menu_now_serifu = 39;
        para_str = para_str + 10;
    } else {
        shougatu_menu_now_serifu = 38;
        para_str = para_str - 10;
    }
    param_clamp();
    sub_sub_step_up();
}
