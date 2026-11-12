/* Address: 0x800713EC */
/* Size: 3992 bytes (998 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolSize: 3992 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=11 IDO52_FORCE_WEB_SPILL=431 IDO52_FORCE_WEB_COLOR=124:v1 IDO52_AS1_TIE_REJECT=898,911,912,818,844,941,968 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: remap_stack_slots --map '32:48' */
#include "types.h"

typedef struct {
    u8 type, a, b, c;
} Sched;

typedef struct {
    s16 pad0;
    s16 f2;
    s16 pad4;
    s16 f6;
    union {
        s32 w;
        struct {
            unsigned b0 : 1, b1 : 1, p2 : 10, b12 : 1, b13 : 1, p14 : 2, b16 : 1, b17 : 1, p18 : 14;
        } f;
    } u;
    u8 pad12[44];
} Chr;

typedef struct {
    unsigned p0 : 25, f25 : 4, b29 : 1, p30 : 2;
} Ecac;

typedef struct {
    u8 pad0[68];
    u8 dayTab[176];
    union {
        u16 h;
        u8 b[2];
    } f854;
    u8 padF6[202];
    Chr chr[11];
    u8 pad428[292];
    Ecac ecac;
    u8 pad550[332];
    Sched sched[32];
    u8 cur;
    u8 cnt;
    u8 flags;
    u8 f7F;
    u8 pad720[2111];
    u8 f6BF;
} Save;

extern Save screen_w;
#define day_cnt screen_w.cnt
#define day_flag screen_w.flags
#define day_flag1 screen_w.f7F
extern u8 season_no, now_year, now_month, now_day, now_youbi;
extern u16 pass_days;
#define basyo_pack screen_w.f854.h
#define byouki_kind screen_w.f854.b[1]
typedef struct {
    unsigned p0 : 9, f9 : 3, p12 : 12, f24 : 8;
} Eb90;
extern Eb90 girl11_flag;
#define reserved_date screen_w.ecac
extern u8 ev_flag14;
extern u8 ev_flag4d;
extern u8 ev_flag52;
extern u8 machiawase_mode, machiawase_basyo;
#define now_girl screen_w.f6BF
extern u8 schedule_cursor;
extern u8 sched_icon;
extern s32 work_tarao;
extern s32 event_girl_no;
extern u8 work_miura[];

extern void goto_step();
extern void goto_sub_step();
extern void season_mess_clear();
extern void return_schedule();

void schedule_day_main()
{
    Save *sv;
    Sched *p;
    s32 i;
    s32 n;
    s32 c;
    u32 x;
    u32 y;
    u32 s;

    chr_flag_clear();
    girls_param_clamp();
    if (now_year >= 98 && now_month >= 3 && now_day != 0) {
        sub_sub_step_up();
        return;
    }
    if (now_year >= 98 && now_month == 2 && now_day == 26 && ev_flag52 == 0) {
        c = 0;
        for (i = 1; i < 10; i++) {
            if (screen_w.chr[i].u.f.b1) {
                c++;
            }
        }
        if (c >= 6) {
            screen_w.cur--;
            event_girl_no = 13;
            work_tarao = 0;
            goto_step(93);
            return;
        }
    }
    if (now_year >= 98 && now_month == 2 && now_day == 28 && girl2_top_check() != 0
        && ev_flag14 == 0) {
        event_girl_no = 2;
        work_tarao = 7;
        ev_flag14++;
        goto_step(93);
        return;
    }
    basyo_flag_set();
    season_flag_set();
    sv = &screen_w;
    if (!(now_month == 12 && now_day == 24) && day_cnt == 0 && now_youbi != 6
        && sv->dayTab[now_day] == 0 && sv->sched[screen_w.cur].type == 251
        && sv->sched[0].type != 64 && sv->sched[screen_w.cur - 1].type == 0) {
        for (i = 0; i < 11; i++) {
            if (sv->chr[i].u.f.b1 && sv->chr[i].f6 >= 76
                && !sv->chr[i].u.f.b16) {
                sv->chr[i].u.f.b16 = 1;
                if (screen_w.cur != 0) {
                    screen_w.cur--;
                }
                day_cnt++;
                screen_w.sched[screen_w.cur].a = i;
                goto_step(89);
                return;
            }
        }
    }
    p = &sv->sched[screen_w.cur];
    x = p->type;
    if (sv->sched[0].type == 37 && sv->sched[screen_w.cur - 1].type == 37) {
        if (x == 50) {
            screen_w.cur++;
            p = &sv->sched[screen_w.cur];
            x = p->type;
        }
        if (x == 51) {
            screen_w.cur++;
            p = &sv->sched[screen_w.cur];
            x = p->type;
        }
    }
    if (x == 50) {
        if (p->a == 0 && (day_flag & 1)) {
            screen_w.cur++;
            p = &sv->sched[screen_w.cur];
        }
        if (p->a != 0 && (day_flag & 2)) {
            screen_w.cur++;
            p = &sv->sched[screen_w.cur];
        }
    }
    x = p->type;
    if (x == 51 && (day_flag & 4)) {
        screen_w.cur++;
        x = screen_w.sched[screen_w.cur].type;
    }
    if (x == 250) {
        screen_w.cur++;
        x = screen_w.sched[screen_w.cur].type;
    }
    if (x == 251 || x == 255) {
        reserved_date_sort();
        if (reserved_date_check(now_year, now_month, now_day) > 0) {
            if (get_g_zyotai_s(reserved_date.f25) == 4) {
                reserved_date.b29 = 0;
                day_flag1 = 1;
                goto_step(67);
                goto_sub_step(2);
                return;
            }
            if (machiawase_mode != machiawase_basyo) {
                reserved_date.b29 = 0;
                day_flag1 = 1;
                goto_step(67);
                goto_sub_step(0);
                return;
            }
            if (reserved_date.b29) {
                reserved_date.b29 = 0;
                day_flag1 = 1;
                goto_step(67);
                goto_sub_step(1);
                return;
            }
        }
    }
    if (screen_w.sched[screen_w.cur].type == 255) {
        s = basyo_pack >> 12;
        if (s >= 6 && s < 11 && (now_day + 6) / 7 == 3 && (sched_icon >> 4) != 4) {
            goto_step(67);
            goto_sub_step(8);
            return;
        }
        if (s < 6 && now_month == 10
            && ((now_year == 95 && now_day == 8) || (now_year == 96 && now_day == 6)
                || (now_year == 97 && now_day == 5))) {
            goto_step(67);
            goto_sub_step(9);
            return;
        }
        if (now_year == 95 && now_month == 10 && now_day == 15) {
            for (i = 0; i < 11; i++) {
                if (screen_w.chr[i].u.f.b1 && screen_w.chr[i].u.f.b12
                    && get_g_zyotai_s(i) != 4 && get_chr_slot_cnt(i) == 0) {
                    day_flag1 = 1;
                    goto_step(67);
                    goto_sub_step(6);
                    return;
                }
            }
        }
        if (now_year == 96 && now_month == 10 && now_day == 13) {
            for (i = 0; i < 11; i++) {
                if (screen_w.chr[i].u.f.b1 && screen_w.chr[i].u.f.b13
                    && get_g_zyotai_s(i) != 4 && get_chr_slot_cnt(i) == 0) {
                    day_flag1 = 1;
                    goto_step(67);
                    goto_sub_step(7);
                    return;
                }
            }
        }
    }
    if (sv->sched[screen_w.cur].type == 251) {
        n = 0;
        for (i = 0; i < 11; i++) {
            if (sv->chr[i].u.f.b1 && sv->chr[i].f6 >= 61 && !sv->chr[i].u.f.b17) {
                sv->chr[i].u.f.b17 = 1;
                n++;
            }
        }
        if (n != 0) {
            goto_step(68);
            return;
        }
        if (day_flag1 == 0) {
            if (now_year == 97 && now_month == 7 && now_day == 26) {
                day_flag1 = 1;
                goto_step(67);
                goto_sub_step(5);
                return;
            }
            if (season_no == 0 && now_year != 98) {
                for (i = 0; i < 11; i++) {
                    if (sv->chr[i].u.f.b1 && sv->chr[i].f6 >= 91) {
                        device_cmd_send(1294);
                        day_flag1 = 1;
                        goto_step(67);
                        goto_sub_step(4);
                        return;
                    }
                }
            }
            if (girl11_flag.f9 != 0 && ev_flag4d == 0) {
                y = girl11_flag.f24;
                if (y < 7 || (y >= 7 && pass_days >= 731 && y < 9)) {
                    if ((rand() & 0x7F) == 0) {
                        day_flag1 = 1;
                        goto_step(67);
                        goto_sub_step(3);
                        return;
                    }
                }
            }
        }
        day_flag1 = 0;
        screen_w.cur++;
        hizuke_advance();
    }
    switch (sv->sched[screen_w.cur].type) {
    case 0:
        for (i = 0; i < 14; i++) {
            if (i != screen_w.sched[screen_w.cur].a) {
                icon_can_use_set(i, 0);
            } else {
                icon_can_use_set(i, 1);
            }
        }
        goto_step(64);
        break;
    case 16:
        goto_step(70);
        break;
    case 254:
        goto_step(56);
        break;
    case 253:
        season_mess_clear();
        goto_step(57);
        break;
    case 32:
        season_mess_clear();
        bzero(work_miura, 256);
        goto_step(114);
        break;
    case 36:
        season_mess_clear();
        bzero(work_miura, 256);
        if ((basyo_pack >> 12) < 6) {
            goto_step(112);
        } else {
            goto_step(113);
        }
        break;
    case 40:
        season_mess_clear();
        goto_step(66);
        break;
    case 44:
        season_mess_clear();
        goto_step(65);
        break;
    case 49:
        goto_step(84);
        break;
    case 48:
        goto_step(85);
        break;
    case 64:
        season_mess_clear();
        goto_step(53);
        break;
    case 65:
        season_mess_clear();
        goto_step(81);
        break;
    case 66:
        goto_step(95);
        break;
    case 67:
        season_mess_clear();
        goto_step(54);
        break;
    case 68:
        day_cnt++;
        if ((byouki_kind & 0xF) == 3) {
            return_schedule(0);
        } else if ((y = screen_w.sched[screen_w.cur - 1].type) == 40) {
            return_schedule(0);
        } else if (y == 71) {
            return_schedule(0);
        } else if (!sv->chr[sv->sched[screen_w.cur].a].u.f.b1) {
            return_schedule(0);
        } else {
            goto_step(89);
        }
        break;
    case 252:
        goto_step(50);
        break;
    case 50:
        goto_step(87);
        break;
    case 51:
        goto_step(86);
        break;
    case 37:
        goto_step(116);
        break;
    case 38:
        goto_step(117);
        break;
    case 39:
        goto_step(118);
        break;
    case 70:
        day_cnt++;
        if (reserved_date.b29) {
            return_schedule(0);
        } else {
            goto_step(90);
        }
        break;
    case 69:
        event_girl_no = sv->sched[screen_w.cur].a;
        work_tarao = screen_w.sched[schedule_cursor].b;
        goto_step(93);
        break;
    case 71:
        day_cnt++;
        goto_step(97);
        break;
    case 72:
        now_girl = sv->sched[screen_w.cur].a;
        sv->chr[now_girl].f2 = 50;
        sv->chr[now_girl].f6 = 0;
        work_tarao = screen_w.sched[screen_w.cur].b;
        goto_step(91);
        break;
    case 73:
        goto_step(88);
        break;
    case 74:
        goto_step(55);
        break;
    case 255:
    default:
        goto_step(52);
        break;
    }
}
