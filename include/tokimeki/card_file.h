#ifndef TOKIMEKI_CARD_FILE_H
#define TOKIMEKI_CARD_FILE_H

/* album_chr (0x8012B600): the 8 KB memory-card file image, a 512-byte PS1
 * card header followed by two 3840-byte records.  The record body is the
 * system-file view (save_sys_set / save_work_restore); game files
 * (save_game_set) lay the same bytes out differently. */

typedef struct {
    unsigned char magic[2];            /* 0x000 'SC' */
    unsigned char type;                /* 0x002 icon flag, 0x13 = 3 frames */
    unsigned char block_entry;         /* 0x003 blocks used (1) */
    unsigned char title[64];           /* 0x004 Shift-JIS title */
    unsigned char reserve[28];         /* 0x044 zeroed */
    unsigned char clut[32];            /* 0x060 icon CLUT */
    unsigned char icon[3][128];        /* 0x080 icon frames */
    struct {
        int sum;            /* +0x000 checksum of bytes 4..3839 */
        struct {            /* system-file view (save_sys_set / save_work_restore);
                               game files (save_game_set) use another layout */
            unsigned char album_status_sav[16];    /* +0x004 */
            unsigned char mascot_flag_sav[16];     /* +0x014 */
            unsigned char sys_flags_sav;           /* +0x024 */
            unsigned char sys_song_no_sav;         /* +0x025 */
            unsigned char last_album_sav;          /* +0x026 */
            unsigned char unk027;              /* +0x027 */
            unsigned char tt_config_sav[20];       /* +0x028 tt_config, tt_result, bunkaken_flags */
            unsigned char taco_rec_sav[24];        /* +0x03C */
            unsigned char opt_flag_sav;            /* +0x054 */
            unsigned char rec_cnt;             /* +0x055 */
            unsigned char save_buf_sav[2048];      /* +0x056 */
            unsigned char name_buf0_sav[16];       /* +0x856 */
            unsigned char name_buf1_sav[16];       /* +0x866 */
            unsigned char name_buf2_sav[16];       /* +0x876 */
            unsigned char save_blk0_sav[128];      /* +0x886 */
            unsigned char save_blk1_sav[128];      /* +0x906 */
            unsigned char save_blk2_sav[256];      /* +0x986 */
            unsigned char sys_idx_tbl_sav[16];     /* +0xA86 */
            unsigned char unkA96[2];           /* +0xA96 */
            unsigned char save_blk3_sav[576];      /* +0xA98 */
            unsigned char save_blk4_sav[256];      /* +0xCD8 */
            unsigned char save_blk5_sav[256];      /* +0xDD8 */
            unsigned char unkED8[40];          /* +0xED8 */
        } sys;
    } rec[2];               /* 0x200, 0x1100 (record 1 = card_save_blk) */
} CARD_FILE;

#endif
