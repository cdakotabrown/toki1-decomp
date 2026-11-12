/* Function: schedule_make */
/* Address: 0x8006D8A4 */
/* Size: 4672 bytes (1168 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 -cckr */
/* @SymbolStart: 0x8006D890 */
/* @SymbolSize: 4692 */
/* @PostProc: strip_leading_instructions --count auto */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=90:t5 IDO52_COPYPROP_VAR=-12:54 IDO52_AS1_NO_PASS1_PROP=93 */
/* @CompileFlags: -cckr */
#include "common.h"

typedef struct {
    u32 b0 : 1;
    u32 b1 : 1;
    u32 b2 : 4;
    u32 b6 : 1;
    u32 rest : 25;
    u8 pad4[52];
} Rec;

typedef struct {
    u8 kind[32];
    u8 event[32];
} Cal;

typedef struct {
    u8 a;
    u8 b;
    u8 c;
    u8 d;
} Cmd;

typedef struct {
    u16 a;
    u16 b;
} Stat;

typedef struct {
    u32 f0 : 25;
    u32 f25 : 2;
    u32 f27 : 5;
} W1688;

typedef struct {
    u8 pad0[61];
    u8 x61;
    u8 x62;
    u8 month;
    u8 day;
    u8 wday;
    u16 days;
    Cal cal[2];
    u8 pad196[48];
    union {
        struct {
            u32 f0 : 18;
            u32 bit18 : 1;
            u32 f19 : 2;
            u32 cnt : 3;
            u32 f24 : 8;
        } w;
        struct {
            u16 f244 : 8;
            u16 f245 : 4;
            u16 girl : 4;
            u16 hi;
        } h;
    } x244;
    u8 pad248[10];
    s16 p258;
    s16 pad260;
    s16 p262;
    s16 pad264;
    s16 p266;
    u8 pad268[41];
    u8 hist[9];
    u8 pad318[138];
    Rec rec[11];
    u8 pad1072[8];
    u8 x1080;
    u8 pad1081[380];
    u8 x1461;
    u8 pad1462[3];
    u8 x1465;
    u8 pad1466[178];
    union {
        Stat stat[12];
        struct {
            u8 pad[44];
            W1688 w;
        } s;
    } u;
    Cmd cmd[563];
    u32 w3944_lo : 4;
    u32 w3944_hi : 28;
} Save;

typedef struct {
    u8 pad[50];
    s16 h50;
    u16 pad52;
    u16 h54;
} Rec9;

extern s32 get_param_top_girl(s32 a0);
extern void schedule_buf_clear(void);
extern void schedule_birthday_add(s32 *a0, s32 a1, s32 a2);
extern void sub_sub_step_up(void);
extern u8 biorhythm_sf_set(s32 a0);
extern s32 final_year_event_check(void);
extern s32 touzyou_check(u8 *a0, u8 *a1);
extern s32 touzyou_check2(u8 *a0, u8 *a1);
extern s32 touzyou_basyo_check(u8 *a0, u8 *a1);
extern void bukatu_reset(void);
extern s32 basyo_range_check(void);
extern s32 birth_day_check_days(s32 a0, s32 a1, s32 a2);
extern u32 get_g_zyotai_h(s32 a0);
extern s32 rand(void);

extern Save screen_w;
extern s8 date_sasoi_judge_tbl0;

#define G screen_w
#define EV   sv->cal[flag].event[pos]
#define KIND sv->cal[flag].kind[pos]
#define CMD  sv->cmd[idx]

void schedule_make(void)
{
    Save *sv;
    s32 day;
    s32 idx;
    s32 pos;
    s32 month;
    s32 year;
    s32 flag;
    s32 i;
    s32 fan;
    s32 count;
    s32 r;
    s32 lose;
    s32 win;
    s32 t;
    s32 dates;
    u8 dateA;
    u8 dateB;
    u8 dated;
    s32 u1;
    s32 u2;
    s32 u3;

    sv = &screen_w;
    dates = 0;
    dated = 0;
    dateA = 0;
    dateB = 0;
    lose = 0;
    win = 0;
    sv->hist[G.w3944_lo % 9]++;
    if (G.x1080 != 0) {
        G.x1080--;
    }
    fan = get_param_top_girl(sv->w3944_lo) & 0xF;
    for (i = 1, count = 0; i < 10; i++) {
        if (sv->rec[i].b1) {
            count++;
        }
    }
    date_sasoi_judge_tbl0 = -1;
    month = G.month;
    year = G.cal[0].kind[0];
    pos = G.day;
    flag = 0;
    if (sv->x244.w.cnt != 0) {
        sv->x244.w.cnt--;
    }
    schedule_buf_clear();
    day = (G.wday + 1) % 7;
    idx = 0;
    if (pos == year) {
        G.cmd[0].a = 254;
        idx++;
        pos = 0;
        flag = 1;
    }
    pos++;
    if (sv->cal[0].event[pos] == 11) {
        CMD.a = 65;
        idx++;
        CMD.a = 16;
        idx++;
        schedule_birthday_add(&idx, flag, pos + 6);
        sub_sub_step_up();
        return;
    }
    if (sv->cal[0].event[pos] == 8) {
        if (sv->x244.h.girl != 11 && sv->w3944_lo == 4) {
            if (sv->x244.h.girl == 6 && (G.u.stat[6].b & 0xF) >= 8) {
                CMD.a = 37;
                idx++;
                schedule_birthday_add(&idx, flag, pos);
                CMD.a = 251;
                idx++;
                CMD.a = 37;
                idx++;
                schedule_birthday_add(&idx, flag, pos + 1);
                CMD.a = 251;
                idx++;
                CMD.a = 37;
                idx++;
                schedule_birthday_add(&idx, flag, pos + 2);
                CMD.a = 251;
                idx++;
                CMD.b = 7;
                CMD.a = 37;
                idx++;
                schedule_birthday_add(&idx, flag, pos + 3);
                CMD.a = 251;
                idx++;
                CMD.b = 7;
                CMD.a = 37;
                idx++;
                schedule_birthday_add(&idx, flag, pos + 4);
                CMD.a = 251;
                idx++;
                CMD.b = 7;
                CMD.a = 37;
                idx++;
                schedule_birthday_add(&idx, flag, pos + 5);
                CMD.a = 251;
                idx++;
                CMD.a = 16;
                idx++;
                schedule_birthday_add(&idx, flag, pos + 6);
                sub_sub_step_up();
                return;
            }
            for (i = 0; i < 6; i++) {
                CMD.a = 66;
                CMD.b = 4;
                CMD.d = fan;
                CMD.c = biorhythm_sf_set(day);
                idx++;
            }
            CMD.a = 16;
            idx++;
            schedule_birthday_add(&idx, flag, pos + 6);
            sub_sub_step_up();
            return;
        }
        if (sv->x244.h.girl == 11) {
            sv->u.s.w.f25 = G.u.s.w.f25 + 1;
        }
    }
    for (; day < 7; day++, pos++) {
        if (year < pos) {
            CMD.a = 254;
            idx++;
            pos = 1;
            flag = 1;
        }
        if (EV == 6) {
            CMD.a = 253;
            idx++;
        }
        if (EV == 14) {
            CMD.a = 253;
            idx++;
        }
        if (EV == 20) {
            CMD.a = 253;
            idx++;
        }
        if (EV == 10) {
            CMD.a = 253;
            idx++;
        }
        if (EV == 16) {
            CMD.a = 253;
            idx++;
        }
        if (EV == 2) {
            CMD.a = 253;
            idx++;
        }
        if (EV == 4) {
            CMD.a = 32;
            idx++;
        } else if (EV == 12) {
            CMD.a = 36;
            idx++;
        } else if (EV == 13) {
            CMD.a = 40;
            idx++;
        } else if (EV == 15) {
            CMD.a = 44;
            idx++;
        } else if (EV == 17) {
            CMD.a = 49;
            idx++;
        } else if (EV == 19) {
            CMD.a = 48;
            idx++;
        } else if (EV == 2 && G.x62 == 96) {
            ((Rec9 *)&G.rec[9])->h54 = 0;
            ((Rec9 *)&G.rec[9])->h50 = 50;
            CMD.b = 10;
            CMD.a = 72;
            idx++;
            bukatu_reset();
        } else if (EV == 2 && G.x62 == 97) {
            CMD.a = 67;
            idx++;
            bukatu_reset();
        } else if (KIND == 1 || KIND == 2) {
            if (final_year_event_check() != 0) {
                G.x1465++;
                CMD.a = 69;
                CMD.b = 11;
                CMD.c = 4;
                idx++;
            } else {
                CMD.a = 16;
                idx++;
            }
        } else if (G.x62 == 97 && month + flag == 13 && pos == 3) {
            if (basyo_range_check() == 1 && (t = sv->x244.h.girl) != 6
                && (sv->u.stat[t].b & 0xF) >= 10) {
                CMD.a = 38;
                idx++;
            } else if (sv->x244.h.girl < 6 && sv->u.stat[sv->x244.h.girl].a >= 380) {
                CMD.a = 39;
                idx++;
            } else {
                CMD.a = 0;
                CMD.b = G.w3944_lo;
                CMD.d = fan;
                if (biorhythm_sf_set(day) != 0) {
                    win++;
                } else {
                    lose++;
                }
                idx++;
            }
        } else if (EV == 18 && G.x62 == 98) {
            CMD.a = 74;
            idx++;
        } else if (KIND == 0 || KIND == 3) {
            if (EV == 5 || G.cmd[0].a == 64) {
                CMD.a = 64;
                idx++;
            } else {
                CMD.a = 0;
                CMD.b = G.w3944_lo;
                CMD.d = fan;
                r = biorhythm_sf_set(day);
                if (r != 0) {
                    win++;
                } else {
                    lose++;
                }
                if (G.w3944_lo == 7) {
                    CMD.c = 0;
                } else {
                    CMD.c = r;
                }
                idx++;
                if (sv->cmd[idx - 1].c == 1 && win + lose == 6 && KIND == 0
                    && (rand() & 0xF) < 2) {
                    CMD.a = 71;
                    CMD.b = 12;
                    idx++;
                }
                if (KIND == 3) {
                    if (G.x61 == 2 && G.days / 365 > G.x1461
                        && G.rec[10].b1 && (get_g_zyotai_h(10) & 0x7F) == 0
                        && (rand() & 0xF) <= 0) {
                        CMD.a = 69;
                        CMD.b = 10;
                        CMD.c = 7;
                        idx++;
                    }
                    if (G.x61 == 1 && day == 2 && flag + G.month == 8
                        && rand() % 100 < 20 && count != 0
                        && (sv->x244.w.bit18 == 0
                            || (G.x62 == 97 && sv->x244.w.bit18 == 0 && pos == 28))) {
                        G.x244.w.bit18 = 1;
                        CMD.a = 73;
                        idx++;
                    }
                }
            }
            if (KIND == 0 && G.cmd[0].a != 64) {
                if ((G.x62 != 95 || G.month != 4) && G.days < 727 && dated == 0) {
                    if (count < 2) {
                        if (touzyou_check(&dateA, &dateB) != 0) {
                            dated++;
                            CMD.a = 72;
                            CMD.b = dateA;
                            CMD.c = dateB;
                            idx++;
                        }
                    } else {
                        if (touzyou_check2(&dateA, &dateB) != 0) {
                            dated++;
                            CMD.a = 72;
                            CMD.b = dateA;
                            CMD.c = dateB;
                            idx++;
                        }
                    }
                    if (G.w3944_lo == 4) {
                        if (touzyou_basyo_check(&dateA, &dateB) != 0) {
                            dated++;
                            CMD.a = 72;
                            CMD.b = dateA;
                            CMD.c = dateB;
                            idx++;
                        }
                    }
                }
                if (dates < 2 && (rand() & 3) == 3) {
                    dates++;
                    CMD.a = 68;
                    idx++;
                }
            }
        }
        if (day == 5 && G.days >= 33) {
            if (G.cmd[0].a == 64) {
                if ((rand() & 1) && G.rec[9].b1 && rand() % 100 < 50
                    && get_g_zyotai_h(9) < 3
                    && G.p266 + G.p258 + G.p262 >= 301) {
                    sv->w3944_lo = 3;
                    CMD.a = 71;
                    CMD.b = 9;
                    idx++;
                }
            } else if (KIND == 0 && G.w3944_lo != 7 && (rand() & 3) != 0
                       && win == 0 && lose == 6) {
                if (fan != 15 && fan) {
                    if (!(G.w3944_lo == 5 && fan == 0)) {
                        CMD.a = 71;
                        CMD.b = fan;
                        idx++;
                    }
                } else {
                    if ((G.w3944_lo == 0 || G.w3944_lo == 1 || G.w3944_lo == 3 || G.w3944_lo == 2) && rand() % 100 < 7
                        && get_g_zyotai_h(9) < 3 && G.rec[9].b1) {
                        CMD.a = 71;
                        CMD.b = 9;
                        idx++;
                    } else if (fan == 0 && G.w3944_lo != 5) {
                        CMD.a = 71;
                        CMD.b = 0;
                        idx++;
                    }
                }
            }
        }
        if (birth_day_check_days(0, G.month + flag, pos) == 1) {
            CMD.a = 50;
            CMD.b = 0;
            idx++;
        }
        for (i = 1; i <= 10; i++) {
            if (sv->rec[i].b1 && sv->rec[i].b6
                && birth_day_check_days(i, G.month + flag, pos) == 1) {
                CMD.a = 50;
                CMD.b = i;
                idx++;
            }
        }
        if (sv->x244.h.f245 != 3 && birth_day_check_days(15, G.month + flag, pos) == 1) {
            CMD.a = 51;
            idx++;
        }
        if (day == 5) {
            CMD.a = 252;
            idx++;
        }
        if (day != 6) {
            CMD.a = 251;
            idx++;
        }
    }
    sub_sub_step_up();
}
