#ifndef TOKIMEKI_TAIIKU_KARI_RUN_H
#define TOKIMEKI_TAIIKU_KARI_RUN_H

/* TAIIKU borrowing race (state 13): taiiku_kari_run[4] at 0x801493F8,
 * one 36-byte runner record each (0 = player, 1..3 CPU).
 * taiiku_race_init clears every field and sets pos.half[1] = i * 8;
 * taiiku_runner_count_inc bumps cnt0c / cnt0e / press_cnt / speed_cnt
 * (and slot 0's tick); taiiku_racer_target_set writes seat;
 * get_taiiku_leader_max sets goal of slots 1..3. */

typedef struct {
    unsigned char active;   /* 0x00 runner in the race */
    unsigned char cue;      /* 0x01 set by taiiku_ouen_cue */
    unsigned char seat;     /* 0x02 seat (class_w[seat + 46]) */
    unsigned char frm;      /* 0x03 pose frame (wraps at 5) */
    unsigned char col;      /* 0x04 frame-colour index (wraps at 5) */
    unsigned char unk05;    /* 0x05 */
    unsigned char goal;     /* 0x06 reached the goal (get_taiiku_leader_max) */
    unsigned char tick;     /* 0x07 frame counter (slot 0, < 41) */
    short unk08;            /* 0x08 */
    short unk0A;            /* 0x0A */
    unsigned short cnt0c;   /* 0x0C */
    unsigned short cnt0e;   /* 0x0E */
    int accel;              /* 0x10 taiiku_accel_set: gain or -768 */
    unsigned int press_cnt; /* 0x14 */
    int speed;              /* 0x18 */
    unsigned int speed_cnt; /* 0x1C */
    union {
        int fix;            /* 0x20 16.16 position */
        short half[2];      /* 0x20 fraction, 0x22 integer (screen y) */
    } pos;
} TAIIKU_KARI_RUN;

typedef char taiiku_kari_run_size_check[sizeof(TAIIKU_KARI_RUN) == 36 ? 1 : -1];

#endif
