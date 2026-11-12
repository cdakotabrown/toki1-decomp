/* @CompileFlags: -Wo,-loopunroll,0 -Wo,-notail */
/* date2_sched_menu_disp - DATE2 overlay, Address: 0x801395A8, Size: 2644 bytes */
#include "types.h"

extern s32 set_kanji_string();
extern s32 strlen(char *);
extern void strcpy(char *, char *);
extern void k_disp_start(s32);

extern u8 date2_sched_no;
extern char **date2_sched2_str_tbl;
extern char **date2_sched0_str_tbl;
extern char **date2_sched3_str_tbl;
extern char **date2_sched1_str_tbl;

void date2_sched_menu_disp(n)
short n;
{
    char buf[81][31];
    s16 i;
    s32 r;
    s32 r2;

    switch (date2_sched_no) {
    case 2:
        for (i = 0; i < 30; i++) {
            strcpy(buf[i], date2_sched2_str_tbl[i]);
        }
        break;
    case 0:
        for (i = 0; i < 51; i++) {
            strcpy(buf[i], date2_sched0_str_tbl[i]);
        }
        break;
    case 3:
        for (i = 0; i < 39; i++) {
            strcpy(buf[i], date2_sched3_str_tbl[i]);
        }
        break;
    default:
        for (i = 0; i < 81; i++) {
            strcpy(buf[i], date2_sched1_str_tbl[i]);
        }
        break;
    }

    if (n % 6 == 0) {
        set_kanji_string(-116, 54, 2, buf[n], 0);
        r = strlen(buf[n]);
        set_kanji_string((s16)(r * 7 - 116), 54, 0, buf[n + 1], 0);
        r2 = strlen(buf[n]);
        r = strlen(buf[n + 1]);
        set_kanji_string((s16)(r * 7 + r2 * 7 - 116), 54, 0,
                         buf[n + 2], 0);
        set_kanji_string(-84, 74, 0, buf[n + 3], 0);
        r = strlen(buf[n + 3]);
        set_kanji_string((s16)(r * 7 - 84), 74, 0, buf[n + 4], 0);
        r2 = strlen(buf[n + 3]);
        r = strlen(buf[n + 4]);
        r = set_kanji_string((s16)(r * 7 + r2 * 7 - 84), 74, 0,
                             buf[n + 5], 0);
        k_disp_start(r);
    } else if (n % 6 == 1) {
        set_kanji_string(-116, 54, 2, buf[n - 1], 0);
        r = strlen(buf[n - 1]);
        set_kanji_string((s16)(r * 7 - 116), 54, 2, buf[n], 0);
        r2 = strlen(buf[n - 1]);
        r = strlen(buf[n]);
        set_kanji_string((s16)(r * 7 + r2 * 7 - 116), 54, 0,
                         buf[n + 1], 0);
        set_kanji_string(-84, 74, 0, buf[n + 2], 0);
        r = strlen(buf[n + 2]);
        set_kanji_string((s16)(r * 7 - 84), 74, 0, buf[n + 3], 0);
        r2 = strlen(buf[n + 2]);
        r = strlen(buf[n + 3]);
        r = set_kanji_string((s16)(r * 7 + r2 * 7 - 84), 74, 0,
                             buf[n + 4], 0);
        k_disp_start(r);
    } else if (n % 6 == 2) {
        set_kanji_string(-116, 54, 2, buf[n - 2], 0);
        r = strlen(buf[n - 2]);
        set_kanji_string((s16)(r * 7 - 116), 54, 2, buf[n - 1], 0);
        r2 = strlen(buf[n - 2]);
        r = strlen(buf[n - 1]);
        set_kanji_string((s16)(r * 7 + r2 * 7 - 116), 54, 2,
                         buf[n], 0);
        set_kanji_string(-84, 74, 0, buf[n + 1], 0);
        r = strlen(buf[n + 1]);
        set_kanji_string((s16)(r * 7 - 84), 74, 0, buf[n + 2], 0);
        r2 = strlen(buf[n + 1]);
        r = strlen(buf[n + 2]);
        r = set_kanji_string((s16)(r * 7 + r2 * 7 - 84), 74, 0,
                             buf[n + 3], 0);
        k_disp_start(r);
    } else if (n % 6 == 3) {
        set_kanji_string(-116, 54, 0, buf[n + 3], 0);
        r = strlen(buf[n + 3]);
        set_kanji_string((s16)(r * 7 - 116), 54, 0, buf[n + 4], 0);
        r2 = strlen(buf[n + 3]);
        r = strlen(buf[n + 4]);
        set_kanji_string((s16)(r * 7 + r2 * 7 - 116), 54, 0,
                         buf[n + 5], 0);
        set_kanji_string(-84, 74, 2, buf[n], 0);
        r = strlen(buf[n]);
        set_kanji_string((s16)(r * 7 - 84), 74, 0, buf[n + 1], 0);
        r2 = strlen(buf[n]);
        r = strlen(buf[n + 1]);
        r = set_kanji_string((s16)(r * 7 + r2 * 7 - 84), 74, 0,
                             buf[n + 2], 0);
        k_disp_start(r);
    } else if (n % 6 == 4) {
        set_kanji_string(-116, 54, 0, buf[n + 2], 0);
        r = strlen(buf[n + 2]);
        set_kanji_string((s16)(r * 7 - 116), 54, 0, buf[n + 3], 0);
        r2 = strlen(buf[n + 2]);
        r = strlen(buf[n + 3]);
        set_kanji_string((s16)(r * 7 + r2 * 7 - 116), 54, 0,
                         buf[n + 4], 0);
        set_kanji_string(-84, 74, 2, buf[n - 1], 0);
        r = strlen(buf[n - 1]);
        set_kanji_string((s16)(r * 7 - 84), 74, 2, buf[n], 0);
        r2 = strlen(buf[n - 1]);
        r = strlen(buf[n]);
        r = set_kanji_string((s16)(r * 7 + r2 * 7 - 84), 74, 0,
                             buf[n + 1], 0);
        k_disp_start(r);
    } else {
        set_kanji_string(-116, 54, 0, buf[n + 1], 0);
        r = strlen(buf[n + 1]);
        set_kanji_string((s16)(r * 7 - 116), 54, 0, buf[n + 2], 0);
        r2 = strlen(buf[n + 1]);
        r = strlen(buf[n + 2]);
        set_kanji_string((s16)(r * 7 + r2 * 7 - 116), 54, 0,
                         buf[n + 3], 0);
        set_kanji_string(-84, 74, 2, buf[n - 2], 0);
        r = strlen(buf[n - 2]);
        set_kanji_string((s16)(r * 7 - 84), 74, 2, buf[n - 1], 0);
        r2 = strlen(buf[n - 2]);
        r = strlen(buf[n - 1]);
        r = set_kanji_string((s16)(r * 7 + r2 * 7 - 84), 74, 2,
                             buf[n], 0);
        k_disp_start(r);
    }
}
