/* @CompileFlags: -Wc,-pic0 -cckr */
/* date2_song_menu_init -- DATE2 overlay, Address: 0x801385CC, Size: 756 bytes (189 insns) */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=120:t0,91:v1 */
#include "types.h"

typedef struct {
    s32 w[21];
} Blk84;

extern Blk84 date2_song_menu_init_tbl;
s16 date2_menu_wid_tbl[4] = {0};
extern u8 date2_song_no;
extern u8 sys_flags;
extern u8 now_icon;
extern u8 step_mode;
extern s32 menu_on;
extern u8 date2_song_menu_init_str0[];
extern u8 date2_song_menu_init_str1[];

extern s32 strlen(u8 *p);
extern void strcpy(u8 *p, u8 *q);
extern void k_reset(s32 a);
extern s32 set_kanji_string(s32 a, s32 b, s32 c, u8 *d, s32 e);
extern void k_disp_start(s32 a);
extern void menu_set(s32 a, s32 b, s16 *c, s16 *d, s16 *e, s16 *f);

void date2_song_menu_init(void)
{
    s32 spTop[3];
    Blk84 buf;
    s16 sp72[4];
    s16 sp64[4];
    s16 sp56[4];
    u8 *p;
    s32 r;
    s16 i;

    buf = date2_song_menu_init_tbl;
    p = (u8 *)&buf;

    sp72[0] = -128; sp64[0] = 52;
    sp64[1] = 52;
    sp72[1] = 0;
    sp72[2] = -128; sp64[2] = 76;
    sp72[3] = -32; sp64[3] = 76;

    r = strlen(p + 21);
    date2_menu_wid_tbl[1] = (s16)((r / 2) << 4);
    sp56[0] = sp56[1] = sp56[2] = sp56[3] = 16;
    date2_menu_wid_tbl[0] = 0;
    date2_menu_wid_tbl[2] = 0;
    date2_menu_wid_tbl[3] = 0;

    k_reset(1);

    if ((sys_flags & 1) || date2_song_no != 0) {
        if (date2_song_no >= 2U) {
            strcpy(p, date2_song_menu_init_str0);
        }
        r = strlen(p);
        date2_menu_wid_tbl[0] = (s16)((r / 2) << 4);
        k_disp_start(set_kanji_string(sp72[0], sp64[0], 0, p, 0));
    }
    k_disp_start(set_kanji_string(sp72[1], sp64[1], 0, p + 21, 0));

    if ((sys_flags & 4) || date2_song_no != 0) {
        r = strlen(p + 42);
        date2_menu_wid_tbl[2] = (s16)((r / 2) << 4);
        k_disp_start(set_kanji_string(sp72[2], sp64[2], 0, p + 42, 0));
    }

    if ((sys_flags & 8) || date2_song_no != 0) {
        if (date2_song_no >= 2U) {
            strcpy(p + 63, date2_song_menu_init_str1);
        }
        r = strlen(p + 63);
        date2_menu_wid_tbl[3] = (s16)((r / 2) << 4);
        k_disp_start(set_kanji_string(sp72[3], sp64[3], 0, p + 63, 0));
    }

    menu_set(0, 4, sp72, sp64, date2_menu_wid_tbl, sp56);

    menu_on = 1;

    for (i = 0; i < 4; i++) {
        if (date2_menu_wid_tbl[i] != 0) {
            now_icon = (u8)i;
            break;
        }
    }

    step_mode++;
}
