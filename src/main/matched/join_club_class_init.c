/* Function: join_club_class_init */
/* Address: 0x8006B6F4 */
/* Size: 676 bytes (169 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=1 IDO52_AS1_TIE_REJECT=562 */
#include "types.h"

extern void class_clear_n(s32 arg0);
extern void set_kanji_string(s16 x, s16 y, s32 a2, u8 *a3, s32 stack0);
extern void menu_set(s32 a0, s32 a1, s16 *a2, s16 *a3, s16 *stack0, s16 *stack1);
extern void k_sub_reset_point_set(void);
extern void sndi(u8 *a0, s32 a1, s32 a2);
extern void k_sub_disp_start(s32 a0);

extern u8 cal_cw_prio;
extern u8 cal_cw_mode;
extern u8 cal_cw_disp;
extern u8 cal_cw_flag4;
extern u8 cal_cw_sel;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s32 cal_cw_hdr;
extern s32 cal_cw_attr;
extern s16 cal_cw_aux14;
extern s16 cal_cw_frame;
extern s16 cal_cw_x;
extern s16 cal_cw_y;

extern s16 basyo_init_tbl3;
extern u8 basyo_init_tbl1;
extern u8 basyo_init_tbl2;
extern u8 _sys_default_tpage_set_tbl2;
extern u8 join_club_class_init_str0;
extern u8 join_club_class_init_str1;
extern u8 join_club_class_init_str2;

extern u8 screen_w[];
extern s32 album_info_disp_tbl1[];

extern u8 menu_csr;

void join_club_class_init(void)
{
    s16 pad0[8];
    s16 spB8[12];
    s16 spA0[12];
    s16 sp88[12];
    s16 sp70[12];
    s32 pad1[2];
    s32 i;
    s32 found;

    class_clear_n(96);

    cal_cw_prio = 4;
    cal_cw_mode = 64;
    cal_cw_attr = 0x40000000;
    cal_cw_disp = 132;
    cal_cw_img = (s32)&basyo_init_tbl1;
    cal_cw_anim_tbl = (s32)&basyo_init_tbl2;
    cal_cw_hdr = (s32)&_sys_default_tpage_set_tbl2;
    cal_cw_aux14 = basyo_init_tbl3;
    cal_cw_flag4 = 8;
    cal_cw_sel = 1;
    cal_cw_frame = 63;
    cal_cw_x = -16;
    cal_cw_y = -19;

    set_kanji_string(-128, -82, 3, &join_club_class_init_str0, 0);

    found = -1;
    i = 0;
    do {
        spB8[i] = ((i / 5) * 84) - 128;
        spA0[i] = ((i % 5) * 16) - 56;
        sp88[i] = 64;
        sp70[i] = 16;

        if ((screen_w[i * 4 + 1647] & 1) == 0) {
            set_kanji_string(spB8[i], spA0[i], 3, (u8 *)album_info_disp_tbl1[i], 0);
            if (found == -1) {
                found = i;
            }
        } else {
            sp88[i] = 0;
        }
        i++;
    } while (i < 11);

    spB8[11] = 40;
    spA0[11] = 8;
    sp88[11] = 64;
    sp70[11] = 16;

    set_kanji_string(spB8[11], spA0[11], 3, &join_club_class_init_str1, 0);

    menu_set(1, 12, spB8, spA0, sp88, sp70);

    menu_csr = found;
    k_sub_reset_point_set();
    sndi(&join_club_class_init_str2, 0, 255);
    k_sub_disp_start(1);
}
