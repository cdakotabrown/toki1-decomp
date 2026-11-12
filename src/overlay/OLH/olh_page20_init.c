/* olh_page20_init - OLH overlay, Address: 0x8013301C, Size: 348 bytes (87 insns) */
#include "types.h"

extern void k_sub_reset(void);
extern s32 set_kanji_string(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void menu_set(s32 a0, s32 a1, s16 *a2, s16 *a3, s16 *a4, s16 *a5);
extern void k_disp_start(s32 a0);
extern void sub_sub_step_up(void);
extern u8 olh_page20_init_str0[];
extern s32 olh_page20_tbl[];

void olh_page20_init(void) {
    s32 x;
    s32 h;
    s32 ret;
    s16 x_buf[8];
    s16 y_buf[8];
    s16 w_buf[8];
    s16 h_buf[8];
    s32 i;

    k_sub_reset();

    set_kanji_string(-80, -64, 15, (s32)olh_page20_init_str0, 0);

    x = -80;
    h = 16;
    i = 0;
    do {
        x_buf[i] = x;
        y_buf[i] = i * 16 - 50;
        w_buf[i] = 192;
        h_buf[i] = h;
        if (i < 4) {
            set_kanji_string(x_buf[i], y_buf[i], 15, olh_page20_tbl[i], 0);
        }
        i++;
    } while (i != 8);

    w_buf[4] = w_buf[5] = w_buf[6] = 0;
    ret = set_kanji_string(x_buf[7], y_buf[7], 15, olh_page20_tbl[7], 0);

    menu_set(0, 8, x_buf, y_buf, w_buf, h_buf);
    k_disp_start(ret);
    sub_sub_step_up();
}
