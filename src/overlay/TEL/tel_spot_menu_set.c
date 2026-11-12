/* @SymbolSize: 3092 */
/* @RodataBase: 0x80140494 */
/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u32 b0 : 1; u32 b1 : 1; u32 b2 : 1; u32 b3 : 1;
    u32 b4 : 1; u32 b5 : 1; u32 b6 : 1; u32 b7 : 1;
    u32 b8 : 1; u32 b9 : 1; u32 b10 : 1; u32 b11 : 1;
    u32 b12 : 1; u32 b13 : 1; u32 b14 : 1; u32 b15 : 1;
    u32 rest : 16;
} SpotFlags;

typedef struct {
    u8 pad0[62];
    u8 year;
    u8 month;
    u8 pad64[224];
    u8 spot[14];
    u8 movie;
    u8 pad303[3637];
    SpotFlags flags;
} TelState;

extern TelState screen_w;
extern s8 now_icon;

extern u8 *machiawase_menu_set_tbl0[];
extern u8 tel_spot_menu_set_str0[];
extern u8 tel_spot_menu_set_str1[];
extern u8 tel_spot_menu_set_str2[];
extern u8 tel_spot_menu_set_str3[];
extern u8 tel_johoshi_look_fmt[];
extern u8 tel_spot_movie_fmt[];

extern u8 cal_cw_prio;
extern u8 cal_cw_mode;
extern u8 cal_cw_disp;
extern u8 cal_cw_flag4;
extern u8 cal_cw_sel;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s16 cal_cw_frame;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s32 cal_cw_hdr;
extern s32 cal_cw_attr;
extern s32 basyo_init_tbl1;
extern s32 basyo_init_tbl2;
extern s32 _sys_default_tpage_set_tbl2;

extern s32 set_kanji_string(s16, s16, u8, u8 *, s32);
extern s32 get_vacation(s32, s32);
extern void menu_set(s32, s32, s16 *, s16 *, s16 *, s16 *);
extern void class_clear_n(s32);
extern void printf();

#define XPOS(i) ((1 - (i) / 5) * -136)
#define YPOS(i) (((i) % 5) * 16 - 56)

void tel_spot_menu_set(s32 page, s32 year, s32 month, s32 day)
{
    s32 i;
    TelState *sv;
    u8 color;
    s16 xpos[11];
    s16 ypos[11];
    s16 flag[11];
    s16 attr[11];

    sv = &screen_w;
    for (i = 0; i < 10; i++) {
        flag[i] = 0;
    }
    xpos[10] = 0; ypos[10] = -84; flag[10] = 100; attr[10] = 16;

    if (page == 0) {
        set_kanji_string(-128, -82, 3, tel_spot_menu_set_str0, 0);
        set_kanji_string(0, -82, 3, tel_spot_menu_set_str1, 0);
        for (i = 0; i < 10; i++) {
            if (sv->spot[i] & 0x7F) {
                flag[i] = 116;
            } else {
                flag[i] = 0;
                switch (i) {
                case 1:
                    if (sv->flags.b1 == 1 && year == 95
                        && month == 6) {
                        flag[i] = 116;
                    }
                    break;
                case 3:
                    if (sv->flags.b3 == 1 && year == 95
                        && month == 12) {
                        flag[i] = 116;
                    }
                    break;
                case 5:
                    if (sv->flags.b5 == 1 && year == 96
                        && month == 6) {
                        flag[i] = 116;
                    }
                    break;
                case 6:
                    if (sv->flags.b7 == 1 && year == 96
                        && month == 12) {
                        flag[i] = 116;
                    }
                    break;
                case 7:
                    if (sv->flags.b2 == 1 && year == 95
                        && month == 9) {
                        flag[i] = 116;
                    }
                    break;
                case 0:
                case 2:
                case 4:
                case 8:
                case 9:
                    break;
                }
            }
            xpos[i] = XPOS(i);
            ypos[i] = YPOS(i);
            attr[i] = 16;
            if (flag[i] != 0) { set_kanji_string(XPOS(i), YPOS(i), 3, machiawase_menu_set_tbl0[i], 0); }
        }
    } else {
        set_kanji_string(-128, -82, 3, tel_spot_menu_set_str2, 0);
        set_kanji_string(0, -82, 3, tel_spot_menu_set_str3, 0);
        printf(tel_johoshi_look_fmt, sv->flags.b14,
                      sv->flags.b13);
        printf(tel_spot_movie_fmt, sv->movie, month,
                      sv->month, year);

        color = 3;
        for (i = 0; i < 6; i++) {
            if (sv->spot[i + 10] & 0x7F) {
                if (i < 3) {
                    flag[i] = 116;
                } else if (i == 3) {
                    if (month == 12 || month == 2 || month == 0
                        || month == 14) {
                    } else if (month == sv->month) {
                        if (sv->flags.b14 == 1) {
                            flag[i] = 116;
                        }
                    } else if (sv->month % color == 2) {
                        if (sv->flags.b13 == 1) {
                            flag[i] = 116;
                        }
                    } else {
                        if (sv->flags.b14 == 1) {
                            flag[i] = 116;
                        }
                    }
                } else if (i == 4) {
                    if (month == sv->month) {
                        if (sv->flags.b14 == 1) {
                            flag[i] = 116;
                        }
                    } else if (sv->month % color == 2) {
                        if (sv->flags.b13 == 1) {
                            flag[i] = 116;
                        }
                    } else {
                        if (sv->flags.b14 == 1) {
                            flag[i] = 116;
                        }
                    }
                } else if (i == 5) {
                    if (month == sv->month) {
                        if (sv->flags.b14 == 1) {
                            flag[i] = 116;
                        }
                    } else if (sv->month % color == 2) {
                        if (sv->flags.b13 == 1) {
                            flag[i] = 116;
                        }
                    } else {
                        if (sv->flags.b14 == 1) {
                            flag[i] = 116;
                        }
                    }
                }
            } else {
                switch (i) {
                case 0:
                    if (sv->flags.b6 == 1 && year == 96
                        && month >= 9) {
                        flag[i] = 116;
                    }
                    break;
                case 1:
                    if (sv->flags.b8 == 1 && year == 97
                        && month >= 3) {
                        flag[i] = 116;
                    }
                    break;
                case 3:
                    if (sv->flags.b13 == 1
                        && sv->month == 2 && month == 3
                        && sv->year == 96) {
                        flag[i] = 116;
                    }
                    break;
                case 4:
                    if (sv->flags.b13 == 1 && year == 95
                        && month == 6 && sv->month == 5) {
                        flag[i] = 116;
                    }
                    break;
                case 2:
                case 5:
                    break;
                }
            }
            xpos[i] = XPOS(i);
            ypos[i] = YPOS(i);
            attr[i] = 16;
            if (flag[i] != 0) {
                set_kanji_string(XPOS(i), YPOS(i), color,
                              machiawase_menu_set_tbl0[i + 10], 0);
            }
        }

        for (i = 6; i < 10; i++) {
            if (i == 6) {
                if (get_vacation(month, day) == 1) {
                    set_kanji_string(XPOS(i), YPOS(i), 3,
                                  machiawase_menu_set_tbl0[16], 0);
                    flag[i] = 116;
                } else if (get_vacation(month, day) == 2) {
                    set_kanji_string(XPOS(i), YPOS(i), 3,
                                  machiawase_menu_set_tbl0[19], 0);
                    flag[i] = 116;
                } else {
                    flag[i] = 0;
                }
            }
            if (i == 7) {
                if (get_vacation(month, day) == 2) {
                    set_kanji_string(XPOS(i), YPOS(i), 3, machiawase_menu_set_tbl0[20], 0);
                    flag[i] = 116;
                } else if (get_vacation(month, day) == 1) {
                    set_kanji_string(XPOS(i), YPOS(i), 3, machiawase_menu_set_tbl0[17], 0);
                    flag[i] = 116;
                } else {
                    flag[i] = 0;
                }
            }
            if (i == 8) {
                flag[i] = 0;
            }
            if (i == 9) {
                if (sv->flags.b15 == 1 && month == 8
                    && (day + 6) / 7 == 1) {
                    set_kanji_string(XPOS(i), YPOS(i), 3, machiawase_menu_set_tbl0[18], 0);
                    flag[i] = 116;
                } else {
                    flag[i] = 0;
                }
            }
            xpos[i] = XPOS(i);
            ypos[i] = YPOS(i);
            attr[i] = 16;
        }
    }

    menu_set(0, 11, xpos, ypos, flag, attr);
    now_icon = 10;
    class_clear_n(96);

    cal_cw_prio = 4;
    cal_cw_mode = 64;
    cal_cw_attr = 0x40000000;
    cal_cw_disp = 132;
    cal_cw_img = (s32)&basyo_init_tbl1;
    cal_cw_anim_tbl = (s32)&basyo_init_tbl2;
    cal_cw_hdr = (s32)&_sys_default_tpage_set_tbl2;
    cal_cw_flag4 = 8;
    cal_cw_sel = 1;
    cal_cw_frame = 64;
    cal_cw_x = -16;
    cal_cw_y = -19;
}
