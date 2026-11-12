/* Function: girls_event_pick */
/* Address: 0x8006FBF0 */
/* Size: 2916 bytes (729 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    union {
        struct {
            u32 b0:1;
            u32 active:1;
            u32 b2:12;
            u32 met:1;
            u32 rest:17;
        } f;
        u8 b[4];
    } u;
    u32 pad4;
    u8 stats[21];
    u8 pad[27];
} Chara;

typedef struct {
    u8 pad[456];
    Chara chars[11];
    u8 pad2[377];
    u8 ev9b;
} Save;

extern Save screen_w;
extern u32 frame_counter;
extern u8 ev_flag00;
extern u8 ev_flag01;
extern u8 ev_flag08;
extern u8 ev_flag0e;
extern u8 ev_flag15;
extern u8 ev_flag1b;
extern u8 ev_flag21;
extern u8 ev_flag22;
extern u8 ev_flag25;
extern u8 ev_flag28;
extern u8 ev_flag2e;
extern u8 ev_flag36;
extern u8 ev_flag3c;
extern u8 ev_flag43;
extern u8 ev_flag44;
extern u8 girl9_flag_hi;
extern u8 denwa_cnt;
extern u8 ev_flag4f;
extern u8 ev_flag50;
extern u8 ev_flag51;
extern u8 ev_flag4d;
extern u8 ev_flag4a;
extern u8 girl11_spot0_cnt;
extern u16 pass_days;
extern u8 ev_flag34;
extern u8 ev_flag35;
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;

typedef struct {
    u32 lo:14;
    u32 flag:1;
    u32 hi:17;
} Bits20;
extern Bits20 girl9_flag;

typedef struct {
    u32 lo:9;
    u32 mood:3;
    u32 hi:20;
} Bits90;
extern Bits90 girl11_flag;

extern u8 get_g_zyotai_h();
extern u8 get_g_zyotai_s();

girls_event_pick(a, b)
u8 *a;
u8 *b;
{
    s32 f0, f1, f2, f3, f4;
    u8 sum[22];
    s32 g0, g1, g2, g3, g4, g5, g6, g7, g8, g9;
    s32 i;
    s32 j;

    *a = 0;
    for (i = 0; i < 11; i++) {
        sum[i] = 0;
        for (j = 0; j < 21; j++) {
            sum[i] += screen_w.chars[i].stats[j];
        }
    }
    for (i = 0; i < 11; i++) {
        if (!screen_w.chars[i].u.f.active) {
            continue;
        }
        if (get_g_zyotai_h(i) >= 2) {
            continue;
        }
        if (sum[i] == 0) {
            continue;
        }
        if ((rand() & 0xF) >= 5) {
            continue;
        }
        switch (i) {
        case 0:
            if (rand() % 100 < 3) {
                if (frame_counter & 1) {
                    if (get_nf_change() == 1) {
                        if (ev_flag00 == 0) {
                            if ((get_g_zyotai_s(i) & 0x7F) < 2) {
                                ev_flag00++;
                                *a = 0;
                                *b = 1;
                                return 1;
                            }
                        }
                    }
                } else if (ev_flag01 == 0) {
                    ev_flag01++;
                    *a = 0;
                    *b = 2;
                    return 1;
                }
            }
            break;
        case 1:
            if (rand() % 100 < 3) {
                if (get_nf_change() == 0) {
                    if (ev_flag08 == 0) {
                        ev_flag08++;
                        *a = 1;
                        *b = 1;
                        return 1;
                    }
                }
            }
            break;
        case 2:
            if (rand() % 100 < 3) {
                if (get_nf_change() == 0) {
                    if (ev_flag0e == 0) {
                        ev_flag0e++;
                        *a = 2;
                        *b = 1;
                        return 1;
                    }
                }
            }
            break;
        case 3:
            if (rand() % 100 < 3) {
                if (get_nf_change() == 1) {
                    if (ev_flag15 == 0) {
                        ev_flag15++;
                        *a = 3;
                        *b = 1;
                        return 1;
                    }
                }
            }
            break;
        case 4:
            if (rand() % 100 < 3) {
                if (get_nf_change() == 0) {
                    ev_flag1b++;
                    *a = 4;
                    *b = 1;
                    return 1;
                }
            }
            break;
        case 5:
            if (rand() % 100 < 3) {
                if (get_nf_change() == 1) {
                    if (ev_flag21 == 0) {
                        ev_flag21++;
                        *a = 5;
                        *b = 1;
                        return 1;
                    }
                }
            }
            if (rand() % 100 < 50) {
                if ((ev_flag25 | 0) == 1) {
                    if (ev_flag22 == 0) {
                        ev_flag22++;
                        *a = 5;
                        *b = 2;
                        return 1;
                    }
                }
            }
            break;
        case 6:
            if (rand() % 100 < 3) {
                if (get_nf_change() == 1) {
                    if (ev_flag28 == 0) {
                        ev_flag28++;
                        *a = 6;
                        *b = 1;
                        return 1;
                    }
                }
            }
            break;
        case 7:
            if (rand() % 100 < 3) {
                if (get_nf_change() == 1) {
                    if (ev_flag2e == 0) {
                        ev_flag2e++;
                        *a = 7;
                        *b = 1;
                        return 1;
                    }
                }
            }
            break;
        case 8:
            if (rand() % 100 < 3) {
                if (get_nf_change() == 0) {
                    if (ev_flag36 == 0) {
                        ev_flag36++;
                        *a = 8;
                        *b = 1;
                        return 1;
                    }
                }
            }
            break;
        case 9:
            if (rand() % 100 < 3) {
                if (get_nf_change() == 1) {
                    if (ev_flag3c == 0) {
                        ev_flag3c++;
                        *a = 9;
                        *b = 1;
                        return 1;
                    }
                }
            }
            if ((ev_flag3c | 0) == 1) {
                if ((girl9_flag.flag | 0) == 1) {
                    if ((screen_w.ev9b | 0) == 1) {
                        screen_w.ev9b++;
                        screen_w.chars[9].u.b[1] &= 0xFFBF;
                        *a = 9;
                        *b = 2;
                        return 1;
                    }
                }
            }
            break;
        case 10:
            if (rand() % 100 < 3) {
                if (get_nf_change() == 1) {
                    if (ev_flag1b != 0) {
                        if (ev_flag43 == 0) {
                            ev_flag43++;
                            *a = 10;
                            *b = 1;
                            return 1;
                        }
                    }
                }
            }
            if (rand() % 100 < 10) {
                if (now_year == 97) {
                    if (now_month == 9) {
                        if (now_day >= 14) {
                            if (ev_flag44 == 0) {
                                ev_flag44++;
                                *a = 10;
                                *b = 2;
                                return 1;
                            }
                        }
                    }
                }
            }
            break;
        }
    }
    if (denwa_cnt >= 21) {
        if (ev_flag50 == 0) {
            if (get_nf_change() == 0) {
                ev_flag50++;
                *a = 12;
                *b = 2;
                return 1;
            }
        }
    }
    if (denwa_cnt >= 41) {
        if (ev_flag4f == 0) {
            ev_flag4f++;
            *a = 12;
            *b = 1;
            return 1;
        }
    }
    if (denwa_cnt >= 65) {
        if (ev_flag51 == 0) {
            ev_flag51++;
            *a = 12;
            *b = 3;
            return 1;
        }
    }
    if (rand() % 100 < 4) {
        if (ev_flag4d == 0) {
            if (girl11_spot0_cnt == 0) {
                if (girl11_flag.mood < 5) {
                    if (pass_days >= 29) {
                        ev_flag4a++;
                        girl11_spot0_cnt = 4;
                        *a = 11;
                        *b = 1;
                        return 1;
                    }
                }
            }
        }
    }
    if (ev_flag35 == 0) {
        if ((ev_flag34 | 0) == 1) {
            ev_flag35++;
            *a = 7;
            *b = 8;
            return 1;
        }
    }
    return 0;
}
