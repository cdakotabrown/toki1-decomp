#ifndef TOKIMEKI_TAIIKU_NININ_RUN_H
#define TOKIMEKI_TAIIKU_NININ_RUN_H

/* TAIIKU three-legged race (state 14): taiiku_ninin_run[4] at 0x8014A32C,
 * one 36-byte pair record each (0 = player pair, 1..3 CPU).
 * taiiku_race_init3 clears them (pos.half[1] = i * 8, run_flag = 1);
 * taiiku_race_count_inc bumps tempo_cnt / step_cnt / tick_a / tick_b;
 * taiiku_race_pose_step uses phase as the call phase and compares
 * tempo_cnt with taiiku_ninin_tempo. */

typedef struct {
    unsigned char phase;    /* 0x00 call phase */
    unsigned char goal;     /* 0x01 reached the goal (get_taiiku_leader) */
    unsigned char pose;     /* 0x02 set by taiiku_race_pose_step */
    unsigned char pad_03;
    unsigned int tempo_cnt; /* 0x04 counted against taiiku_ninin_tempo */
    int accel;              /* 0x08 taiiku_race_rate_set: +/- gain */
    int speed;              /* 0x0C accumulated by taiiku_race_scroll_accum */
    int step_cnt;           /* 0x10 11-tick timer */
    union {
        int fix;            /* 0x14 16.16 position */
        short half[2];      /* 0x14 fraction, 0x16 integer (screen y) */
    } pos;
    unsigned short tick_a;  /* 0x18 (>= 50 ends the run) */
    unsigned short tick_b;  /* 0x1A */
    int run_flag;           /* 0x1C non-zero while running */
    int unk20;              /* 0x20 */
} TAIIKU_NININ_RUN;

typedef char taiiku_ninin_run_size_check[sizeof(TAIIKU_NININ_RUN) == 36 ? 1 : -1];

#endif
