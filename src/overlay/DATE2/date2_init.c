/* @RodataBase: 0x8013A328 */
/* Address: 0x80138238 */
/* Size: 728 bytes (182 insns) */
/* @SymbolStart: 0x8013820C */
/* @SymbolSize: 768 */
#include "types.h"

extern void k_reset(s32);
extern void basyo_init(s32);
extern void date2_ptr_tbl_init2(void);
extern void sub_sub_step_up(void);

extern u8  now_girl;
extern s16 ans[];
extern s16 kao2[];
extern s32 serifu_no;
extern u8  date2_song_no;
extern s16 date2_menu_prev;
extern s16 date2_song_now_s;
extern s16 date2_song_now_s_in;
extern u8  date2_init_str0[];
extern s32 date2_song_res_tbl[][11];
extern s32 date2_song_ptr[];

void date2_init(void) {
    ans[0] = 0;
    ans[1] = 0;
    ans[2] = 0;
    kao2[0] = 1;
    kao2[1] = 1;
    kao2[2] = 1;
    k_reset(0);
    basyo_init((s32)date2_init_str0);
    serifu_no = 0;
    date2_song_no = 0;
    date2_menu_prev = 0;
    date2_ptr_tbl_init2();
    switch (now_girl) {
    case 0:
        date2_song_ptr[0] = date2_song_res_tbl[0][1];
        date2_song_ptr[1] = date2_song_res_tbl[1][1];
        date2_song_ptr[2] = date2_song_res_tbl[2][1];
        break;
    case 1:
        date2_song_ptr[0] = date2_song_res_tbl[0][5];
        date2_song_ptr[1] = date2_song_res_tbl[1][5];
        date2_song_ptr[2] = date2_song_res_tbl[2][5];
        break;
    case 2:
        date2_song_ptr[0] = date2_song_res_tbl[0][2];
        date2_song_ptr[1] = date2_song_res_tbl[1][2];
        date2_song_ptr[2] = date2_song_res_tbl[2][2];
        break;
    case 3:
        date2_song_ptr[0] = date2_song_res_tbl[0][3];
        date2_song_ptr[1] = date2_song_res_tbl[1][3];
        date2_song_ptr[2] = date2_song_res_tbl[2][3];
        break;
    case 4:
        date2_song_ptr[0] = date2_song_res_tbl[0][9];
        date2_song_ptr[1] = date2_song_res_tbl[1][9];
        date2_song_ptr[2] = date2_song_res_tbl[2][9];
        break;
    case 5:
        date2_song_ptr[0] = date2_song_res_tbl[0][6];
        date2_song_ptr[1] = date2_song_res_tbl[1][6];
        date2_song_ptr[2] = date2_song_res_tbl[2][6];
        break;
    case 6:
        date2_song_ptr[0] = date2_song_res_tbl[0][7];
        date2_song_ptr[1] = date2_song_res_tbl[1][7];
        date2_song_ptr[2] = date2_song_res_tbl[2][7];
        break;
    case 7:
        date2_song_ptr[0] = date2_song_res_tbl[0][4];
        date2_song_ptr[1] = date2_song_res_tbl[1][4];
        date2_song_ptr[2] = date2_song_res_tbl[2][4];
        break;
    case 8:
        date2_song_ptr[0] = date2_song_res_tbl[0][0];
        date2_song_ptr[1] = date2_song_res_tbl[1][0];
        date2_song_ptr[2] = date2_song_res_tbl[2][0];
        break;
    case 9:
        date2_song_ptr[0] = date2_song_res_tbl[0][8];
        date2_song_ptr[1] = date2_song_res_tbl[1][8];
        date2_song_ptr[2] = date2_song_res_tbl[2][8];
        break;
    case 10:
        date2_song_ptr[0] = date2_song_res_tbl[0][10];
        date2_song_ptr[1] = date2_song_res_tbl[1][10];
        date2_song_ptr[2] = date2_song_res_tbl[2][10];
        break;
    }
    date2_song_now_s = 0;
    date2_song_now_s_in = 0;
    sub_sub_step_up();
}
