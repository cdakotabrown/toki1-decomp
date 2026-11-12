/* Address: 0x80134FC8 */
/* Size: 180 bytes (45 insns) */
/* @SymbolSize: 180 */
#include "types.h"
#include "tokimeki/class_w.h"

extern u8 mes_win_cw_prio;
extern u8 mes_win_cw_mode;
extern u8 mes_win_cw_disp;
extern u8 mes_win_cw_flag4;
extern u8 mes_win_cw_sel;
extern s32 mes_win_cw_img;
extern s32 mes_win_cw_anim_tbl;
extern s16 mes_win_cw_x;
extern s16 mes_win_cw_y;
extern s32 mes_win_cw_hdr;
extern s32 mes_win_cw_attr;

extern s32 tpage_set_adr;
extern s32 img_set_adr;
extern s32 anim_set_adr;
extern CLASS_W class_w[];

extern void class_clear_n(s32);

void taco_obj_init(void) {
    class_clear_n(1);
    mes_win_cw_prio = 8;
    mes_win_cw_attr = 0;
    mes_win_cw_mode = 64;
    mes_win_cw_disp = 132;
    mes_win_cw_img = img_set_adr;
    mes_win_cw_anim_tbl = anim_set_adr;
    mes_win_cw_hdr = tpage_set_adr;
    mes_win_cw_flag4 = 8;
    mes_win_cw_sel = 129;
    class_w[1].clut = 15;
    mes_win_cw_x = -160;
    mes_win_cw_y = -100;
}
