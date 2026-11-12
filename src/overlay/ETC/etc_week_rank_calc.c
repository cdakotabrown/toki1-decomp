/* @CompileFlags: -cckr */
/* Address: 0x801480BC, Size: 2696 bytes (674 insns) */
#include "types.h"

extern void printf();
extern s32 rand();
extern s32 etc_month_record_check();
extern s32 rcos();

typedef struct {
    s16 v;
    u8 pad[54];
} GirlRec;

typedef struct {
    u8 pad0[446];
    GirlRec rec[20];
    u8 pad1[1588 - 446 - 20 * 56];
    u8 seen[16];
} SaveData;

extern SaveData screen_w;
extern s32 siken_total;
extern u8 etc_week_base_tbl[];
extern u8 etc_week_rnd_tbl[];
extern s16 etc_rank_score[14];
extern s32 etc_rank_order[14];
extern s32 etc_rank_juni[14];

extern u8 etc_week_rank_calc_str0[];
extern u8 etc_rank_score_fmt[];
extern u8 etc_week_rank_calc_str1[];
extern u8 etc_rank_order_fmt[];
extern u8 etc_week_rank_calc_str2[];
extern u8 etc_rank_juni_calc_fmt[];
extern u8 etc_week_rank_calc_str3[];
extern u8 etc_rank_juni_fmt[];
extern u8 etc_week_rank_calc_str4[];
extern u8 etc_rank_juni_adj_fmt[];

s32 etc_week_rank_calc()
{
    s32 i;
    s32 k;
    s32 changed;
    s32 v;
    s32 r1;
    s32 r2;
    s32 max;
    s32 idx;
    s32 rnd;
    s32 val;
    s32 angle;
    s32 min;
    s32 tmp[14];

    printf(etc_week_rank_calc_str0);

    for (i = 0; i < 13; i++) {
        val = rand();
        r1 = (val % (s32)etc_week_rnd_tbl[i]) * ((rand() & 1) * 2 - 1);
        val = rand();
        r2 = (val % (s32)etc_week_rnd_tbl[i]) * ((rand() & 1) * 2 - 1);
        etc_rank_score[i] = (etc_week_base_tbl[i] + r2) * 5
                        + etc_month_record_check(i) * (rand() % 10 + 1) + r1;
        if (etc_rank_score[i] > 500) {
            etc_rank_score[i] = 500;
        }
        printf(etc_rank_score_fmt, i, etc_rank_score[i]);
    }

    etc_rank_score[13] = siken_total;

    do {
        changed = 0;
        for (k = 0; k < 14; k++) {
            for (i = k + 1; i < 14; i++) {
                if (etc_rank_score[i] == etc_rank_score[k]) {
                    changed++;
                    etc_rank_score[i]++;
                }
            }
        }
    } while (changed);

    for (i = 0; i < 14; i++) {
        tmp[i] = etc_rank_score[i];
    }

    for (k = 0; k < 14; k++) {
        idx = 0;
        max = tmp[0];
        for (i = 1; i < 14; i++) {
            if (max < tmp[i]) {
                max = tmp[i];
                idx = i;
            }
        }
        tmp[idx] = 0;
        etc_rank_order[k] = idx;
    }

    printf(etc_week_rank_calc_str1);
    for (i = 0; i < 14; i++) {
        printf(etc_rank_order_fmt, i, etc_rank_order[i]);
    }

    printf(etc_week_rank_calc_str2);
    rnd = rand() & 3;

    for (i = 0; i < 14; i++) {
        angle = (etc_rank_score[i] << 12) / 1000;
        val = rcos(angle);
        etc_rank_juni[i] = val * 200 / 4096 + rnd + 200;
        printf(etc_rank_juni_calc_fmt, i, angle, val, etc_rank_juni[i]);
    }

    do {
        changed = 0;
        for (k = 0; k < 14; k++) {
            for (i = k + 1; i < 14; i++) {
                if (etc_rank_juni[i] == etc_rank_juni[k]) {
                    changed++;
                    if (etc_rank_juni[i] < etc_rank_juni[k]) {
                        etc_rank_juni[k]++;
                    } else {
                        etc_rank_juni[i]++;
                    }
                }
            }
        }
    } while (changed);

    printf(etc_week_rank_calc_str3);
    for (i = 0; i < 14; i++) {
        printf(etc_rank_juni_fmt, i, etc_rank_juni[i]);
    }

    min = 1000;
    for (i = 0; i < 14; i++) {
        if (etc_rank_juni[i] < min) {
            min = etc_rank_juni[i];
        }
    }

    if (min >= 9) {
        v = rand() % (min - 8) + min - 8;
        for (i = 0; i < 14; i++) {
            etc_rank_juni[i] -= v;
        }
    } else if (min <= 0) {
        v = 1 - min;
        for (i = 0; i < 14; i++) {
            etc_rank_juni[i] += v;
        }
    }

    max = etc_rank_score[0];
    idx = 0;
    for (i = 1; i < 14; i++) {
        if (max < etc_rank_score[i]) {
            max = etc_rank_score[i];
            idx = i;
        }
    }

    if (max >= 466) {
        rnd = etc_rank_juni[idx] - 1;
        for (i = 0; i < 14; i++) {
            etc_rank_juni[i] -= rnd;
        }
    }

    printf(etc_week_rank_calc_str4);
    for (i = 0; i < 14; i++) {
        printf(etc_rank_juni_adj_fmt, i, etc_rank_juni[i]);
    }

    if (etc_rank_order[0] == 13 && etc_rank_order[1] == 0
        && etc_rank_juni[0] < etc_rank_juni[13]) {
        max = etc_rank_juni[0];
        etc_rank_juni[0] = etc_rank_juni[13];
        etc_rank_juni[13] = max;
    }

    if (etc_rank_order[0] == 13 && etc_rank_juni[13] <= 0) {
        v = 1 - etc_rank_juni[13];
        etc_rank_juni[13] = 1;
        for (i = 0; i < 13; i++) {
            etc_rank_juni[i] += v;
        }
    }

    for (i = 0; i < 11; i++) {
        if (etc_rank_score[i] < etc_rank_score[13]) {
            if (screen_w.seen[i] == 0) {
                screen_w.rec[i].v += 11;
                screen_w.seen[i] = 1;
            } else {
                screen_w.rec[i].v += 2;
            }
        }
    }

    if (etc_rank_juni[13] == 1) {
        for (i = 0; i < 11; i++) {
            screen_w.rec[i].v += 2;
        }
    }
}
