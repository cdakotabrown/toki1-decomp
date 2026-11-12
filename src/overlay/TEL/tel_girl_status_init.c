/* @CompileFlags: -cckr */
#include "types.h"

extern s32 set_kanji_string(s16, s16, u8, u8 *, s32);
extern void class_clear_n(s32);

typedef struct {
    u8  f0;
    u8  f1;
    u8  f2;
    u8  f3;
    u8  f4;
    u8  f5;
    u8  f6;
    u8  pad7[5];
    s32 *p12;
    s32 *p16;
    s16 f20;
    u8  pad22[2];
    s16 f24;
    u8  pad26[12];
    s16 x;
    u8  pad40[2];
    s16 y;
    u8  pad44[8];
    s32 *p52;
    s32 f56;
    u8  pad60[8];
} Rec;

typedef struct {
    unsigned b0 : 1;
    unsigned met : 1;
    unsigned rest : 30;
} Flags;

typedef struct {
    u8  pad0[6];
    s16 days;
    Flags flags;
    u8  pad12[44];
} Chr;

typedef struct {
    u8  pad0[448];
    Chr chr[11];
    u8  pad1064[240];
    u8  level[11];
} Save;

extern Rec class_w[];
extern Save screen_w;
extern s16 cal_cw_frame;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s16 basyo_init_tbl3;
extern s32 _sys_default_tpage_set_tbl2;
extern s32 basyo_init_tbl2;
extern s32 basyo_init_tbl1;
extern u8  tel_girl_status_init_str0[];
extern u8  tel_girl_status_init_str1[];
extern s32 girl_full_name_tbl[];
extern s32 work_fuku[];

void tel_girl_status_init()
{
    s32 i;
    s32 idx;
    s32 level;
    s32 xbase;
    s32 col;
    s32 ybase;
    u8 score[16];
    s16 rank[16];
    s32 days;

    for (i = 0; i < 34; i++) {
        class_clear_n(i + 96);
        class_w[i + 96].f1 = 4;
        class_w[i + 96].f2 = 64;
        class_w[i + 96].f56 = 0x40000000;
        class_w[i + 96].f3 = 132;
        class_w[i + 96].p12 = &basyo_init_tbl1;
        class_w[i + 96].p16 = &basyo_init_tbl2;
        class_w[i + 96].p52 = &_sys_default_tpage_set_tbl2;
        class_w[i + 96].f20 = basyo_init_tbl3;
        class_w[i + 96].f4 = 8;
        class_w[i + 96].f5 = 1;
        class_w[i + 96].x = 512;
    }
    cal_cw_frame = 66;
    cal_cw_x = -16;
    cal_cw_y = -27;
    work_fuku[1] = set_kanji_string(-128, -98, 4, tel_girl_status_init_str0, 0);
    idx = 97;
    get_grp(score, rank);
    for (i = 0; i < 11; i++) {
        screen_w.level[i] = 0;
        if (screen_w.chr[i].flags.met) {
            xbase = i / 6 * 130;
            col = i % 6 * 16;
            set_kanji_string(xbase - 130, col - 72, 3, girl_full_name_tbl[i], 0);
            ybase = col - 68;
            class_w[idx].f24 = 97; class_w[idx].x = xbase - 50; class_w[idx].y = ybase;
            idx++;
            if (rank[i] >= 500) {
                level = 0;
            } else if (score[i] < 6) {
                level = 6;
            } else if (score[i] < 11) {
                level = 5;
            } else if (score[i] < 21) {
                level = 4;
            } else if (score[i] < 41) {
                level = 3;
            } else if (score[i] < 61) {
                level = 2;
            } else {
                level = 1;
            }
            days = screen_w.chr[i].days;
            class_w[idx].f24 = level + 89; class_w[idx].x = xbase - 34; class_w[idx].y = ybase;
            idx++;
            if (days >= 60) {
                class_w[idx].f6 = -(abs(days - 60) / 10) * 2 + 8;
                if (class_w[idx].f6 == 0) {
                    class_w[idx].f6 = 1;
                }
                class_w[idx].f24 = 96;
                class_w[idx].x = xbase - 18;
                class_w[idx].y = ybase;
                idx++;
            }
            screen_w.level[i] = level + 1;
        }
    }
    work_fuku[1] = set_kanji_string(-128, -98, 4, tel_girl_status_init_str1, 0);
}
