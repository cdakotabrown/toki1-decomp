/* olh_trivia_page_init - OLH overlay, Address: 0x80135B3C, Size: 296 bytes (74 insns) */
#include "types.h"

extern u8 olh_main70_0_str0[];
extern s32 olh_page70_tbl[];

extern void k_sub_reset(void);
extern s32 set_kanji_string(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void menu_set(s32 a0, s32 a1, s16 *a2, s16 *a3, s16 *a4,
                          s16 *a5);
extern void k_disp_start(s32 a0);
extern void sub_sub_step_up(void);

void olh_trivia_page_init(void)
{
    s32 id;
    s32 unused1;
    s32 unused2;
    s16 x[8];
    s16 y[8];
    s16 w[8];
    s16 h[8];
    s32 i;

    k_sub_reset();
    set_kanji_string(-80, -64, 15, (s32)olh_main70_0_str0, 0);
    for (i = 0; i < 8; i++) {
        x[i] = -80;
        y[i] = i * 16 - 50;
        w[i] = 192;
        h[i] = 16;
        id = set_kanji_string(x[i], y[i], 15, olh_page70_tbl[i], 0);
    }
    menu_set(0, 8, x, y, w, h);
    k_disp_start(id);
    sub_sub_step_up();
}
