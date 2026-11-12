#ifndef TOKIMEKI_SAVE_WORK_H
#define TOKIMEKI_SAVE_WORK_H

/* Records in the save work block at 0x800F01F0 (album_work, 3808 bytes):
 * tt_config (0x800F021C) is the TT minigame high-score table, taco_rec
 * (0x800F0230) the TACO result record.  unkNN fields are used but not yet
 * named.  Plain C types: this header is included by files that use either
 * types.h or tokimeki/types.h. */

typedef struct {
    unsigned short score[3];           /* 0x00 record table values */
    unsigned char name[3][3];          /* 0x06 3-letter initials per entry */
    unsigned char unk0F;               /* 0x0F TT st[68], default 7 */
    unsigned char unk10;               /* 0x10 TT st[69], default 6 */
    unsigned char flags;               /* 0x11 bit0 = st[67], bit1 = st[73] */
} TT_CONFIG;

typedef struct {
    unsigned char flags;               /* 0x00 b0 valid, b1, b2 (D_8015EDE8&1), b3:2 (D_8015EDEC%3), b5:3 */
    unsigned char unk01;               /* 0x01 lo:4, hi:4 (taco_shot_btn%3) */
    unsigned char name[3][3];          /* 0x02 */
    unsigned char unk0B;               /* 0x0B */
    int score[3];           /* 0x0C */
} TACO_REC;

#endif
