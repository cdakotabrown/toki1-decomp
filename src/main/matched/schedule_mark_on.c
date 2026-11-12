/* Function: schedule_mark_on */
/* Address: 0x80068030 */
/* Size: 2348 bytes (587 instructions) */
/* Region: Game (IDO 5.2, K&R mode) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_insn_reg_field 0xe4:rt:v1:v0 0xe8:rs:v1:v0 0xe8:rt:v1:v0 0xf4:rt:v1:v0 0xfc:rs:v1:v0 */
/* @PostProc: swap_instructions --offset1 0x100 --offset2 0x104 */
/* @PostProc: swap_instructions --offset1 0x104 --offset2 0x10c */
/* @PostProc: swap_branch_operands --offsets 0x108 */
/* @PostProc: swap_instructions --offset1 0x654 --offset2 0x658 */
/* @CompileFlags: -cckr -Wc,-pic1 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=8,98,1,128 IDO52_FORCE_WEB_COLOR=3:s1,105:s3,74:s3,30:s2,46:s4,37:s5 */
#include "types.h"

typedef struct {
    u8  b0, b1, b2, b3, b4, b5, b6, b7;
    u8  pad08[4];
    void *p12;
    void *p16;
    s16 h20;
    s16 pad22;
    s16 type;
    u8  pad26[12];
    s16 x;
    s16 pad40;
    s16 y;
    u8  pad44[8];
    void *p52;
    s32 w56;
    u8  pad60[8];
} Chr68;

typedef struct {
    unsigned month : 4;
    unsigned day : 5;
    unsigned : 23;
} CalDate;

typedef struct {
    unsigned : 6;
    unsigned on : 1;
    unsigned : 25;
    CalDate date;
    u8 pad[48];
} CalBirth;

typedef struct {
    unsigned : 12;
    unsigned month : 4;
    unsigned day : 5;
    unsigned : 8;
    unsigned on : 1;
    unsigned : 2;
} CalHoliday;

typedef struct {
    u8 count;
    u8 pad01[31];
    u8 wday;
    u8 sched[31];
} CalSched;

typedef struct {
    u8 year;
    u8 month;
    u8 day;
    u8 pad[5];
} CalEvent;

typedef struct {
    u8  pad000[0x3E];
    u8  year;
    u8  month;
    u8  pad040[4];
    CalSched sched[2];
    u8  pad0C4[0x30];
    struct {
        unsigned : 12;
        unsigned month : 4;
        unsigned : 16;
    } now;
    CalDate today;
    u8  pad0FC[0x1C8 - 0xFC];
    CalBirth birth[11];
    u8  pad430[0x54C - 0x430];
    CalHoliday girl_menu_select[8];
    u8  pad56C[0x762 - 0x56C];
    CalEvent event[256];
} CalBlock;

extern CalBlock screen_w;
extern Chr68 class_w[];
extern u8 schedule_mark_on_tbl0[];
extern s32 basyo_init_tbl1;
extern s32 basyo_init_tbl2;
extern s32 _sys_default_tpage_set_tbl2;
extern s16 basyo_init_tbl3;

extern void class_clear_n(s32);
extern s16 get_calx_pos(s32, s32);
extern s16 get_caly_pos(s32, s32);

#define ADD_MARK(t, who, day) \
    class_w[cnt + 97].type = (t); \
    class_w[cnt + 97].x = get_calx_pos((who), (day)); \
    class_w[cnt + 97].y = get_caly_pos((who), (day)); \
    cnt++

void schedule_mark_on(void)
{
    CalBlock *cal;
    s32 i;
    s32 cnt;
    s32 rep;
    s32 mark;
    s32 m;
    s32 d;
    u32 j;
    s32 v;
    s32 k;

    cal = &screen_w;
    for (i = 0; i < 32; i++) {
        class_clear_n(i + 97);
        class_w[i + 97].b1 = 3;
        class_w[i + 97].b2 = 64;
        class_w[i + 97].w56 = 0;
        class_w[i + 97].b3 = 140;
        class_w[i + 97].p12 = &basyo_init_tbl1;
        class_w[i + 97].p16 = &basyo_init_tbl2;
        class_w[i + 97].p52 = &_sys_default_tpage_set_tbl2;
        class_w[i + 97].h20 = basyo_init_tbl3;
        class_w[i + 97].b4 = 8;
        class_w[i + 97].b5 = 1;
        class_w[i + 97].b6 = 8;
        class_w[i + 97].type = 98;
        class_w[i + 97].x = 256;
        class_w[i + 97].y = 256;
        class_w[i + 97].b7 = 128;
    }

    m = screen_w.today.month;
    d = screen_w.today.day;
    rep = 0;
    cnt = 0;
    if (m == screen_w.month) {
        i = d - 1;
        ADD_MARK(110, 0, i);
    }
    if (m == screen_w.month + 1) {
        i = d - 1;
        ADD_MARK(110, 1, i);
    }

    for (j = 0; j < 11; j++) {
        m = screen_w.birth[j].date.month;
        d = screen_w.birth[j].date.day;
        if (screen_w.birth[j].on) {
            if (m == screen_w.month) {
                i = d - 1;
                ADD_MARK(109, 0, i);
            }
            v = screen_w.month + 1;
            if (m == v || m + 12 == v) {
                i = d - 1;
                ADD_MARK(109, 1, i);
            }
        }
    }

    for (j = 0; j < 8; j++) {
        m = screen_w.girl_menu_select[j].month;
        d = screen_w.girl_menu_select[j].day;
        if (screen_w.girl_menu_select[j].on) {
            if (m == screen_w.month) {
                i = d - 1;
                ADD_MARK(98, 0, i);
            }
            if (m == screen_w.month % 12 + 1) {
                i = d - 1;
                ADD_MARK(98, 1, i);
            }
        }
    }

    if (screen_w.now.month >= 6 && screen_w.now.month < 11) {
        v = 21 - screen_w.sched[0].wday;
        m = 21 - screen_w.sched[1].wday;
        i = v - 1;
        ADD_MARK(111, 0, i);
        if (screen_w.year != 98 || screen_w.month != 2) {
            i = m - 1;
            ADD_MARK(111, 1, i);
        }
    }

    j = 0;
    if (screen_w.sched[0].count != 0) {
        do {
            if (screen_w.sched[0].sched[j] != 0 &&
                schedule_mark_on_tbl0[screen_w.sched[0].sched[j]] != 255) {
                if (schedule_mark_on_tbl0[screen_w.sched[0].sched[j]] == 111) {
                    if (screen_w.month == 8 && screen_w.now.month < 11) {
                        ADD_MARK(schedule_mark_on_tbl0[screen_w.sched[0].sched[j]], 0, j);
                    }
                } else {
                    ADD_MARK(schedule_mark_on_tbl0[screen_w.sched[0].sched[j]], 0, j);
                }
                if (schedule_mark_on_tbl0[screen_w.sched[0].sched[j]] == 102 && screen_w.month != 2) {
                    mark = schedule_mark_on_tbl0[screen_w.sched[0].sched[j]];
                    rep = 6;
                } else if (schedule_mark_on_tbl0[screen_w.sched[0].sched[j]] == 108) {
                    mark = schedule_mark_on_tbl0[screen_w.sched[0].sched[j]];
                    rep = 6;
                } else if (schedule_mark_on_tbl0[screen_w.sched[0].sched[j]] == 103) {
                    mark = schedule_mark_on_tbl0[screen_w.sched[0].sched[j]];
                    rep = 7;
                } else if (schedule_mark_on_tbl0[screen_w.sched[0].sched[j]] == 111 &&
                           screen_w.month == 8 && screen_w.now.month < 11) {
                    mark = schedule_mark_on_tbl0[screen_w.sched[0].sched[j]];
                    rep = 6;
                }
            } else if (rep > 0) {
                ADD_MARK(mark, 0, j);
            }
            rep--;
            if (cnt + 97 >= 160) {
                return;
            }
            j++;
        } while (j < cal->sched[0].count);
        rep = 0;
        j = 0;
    }

    j = 0;
    if (screen_w.sched[1].count != 0) {
        do {
            if (screen_w.sched[1].sched[j] != 0 &&
                schedule_mark_on_tbl0[screen_w.sched[1].sched[j]] != 255) {
                if (schedule_mark_on_tbl0[screen_w.sched[1].sched[j]] == 111) {
                    if (screen_w.month == 7 && screen_w.now.month < 11) {
                        ADD_MARK(schedule_mark_on_tbl0[screen_w.sched[1].sched[j]], 1, j);
                    }
                } else {
                    ADD_MARK(schedule_mark_on_tbl0[screen_w.sched[1].sched[j]], 1, j);
                }
                if (schedule_mark_on_tbl0[screen_w.sched[1].sched[j]] == 102 && screen_w.month != 1) {
                    mark = schedule_mark_on_tbl0[screen_w.sched[1].sched[j]];
                    rep = 6;
                } else if (schedule_mark_on_tbl0[screen_w.sched[1].sched[j]] == 108) {
                    mark = schedule_mark_on_tbl0[screen_w.sched[1].sched[j]];
                    rep = 6;
                } else if (schedule_mark_on_tbl0[screen_w.sched[1].sched[j]] == 103) {
                    mark = schedule_mark_on_tbl0[screen_w.sched[1].sched[j]];
                    rep = 7;
                } else if (schedule_mark_on_tbl0[screen_w.sched[1].sched[j]] == 111 &&
                           screen_w.month == 7 && screen_w.now.month < 11) {
                    mark = schedule_mark_on_tbl0[screen_w.sched[1].sched[j]];
                    rep = 6;
                }
            } else if (rep > 0) {
                ADD_MARK(mark, 1, j);
            }
            rep--;
            if (cnt + 97 >= 160) {
                return;
            }
            j++;
        } while (j < cal->sched[1].count);
    }

    for (k = 0; k < 256; k++) {
        v = screen_w.event[k].year;
        m = screen_w.event[k].month;
        d = screen_w.event[k].day;
        if (v == screen_w.year && m == screen_w.month) {
            i = d - 1;
            ADD_MARK(98, 0, i);
        }
        if (cnt + 97 >= 160) {
            return;
        }
        if (screen_w.event[k].year == 0) {
            return;
        }
    }
}
