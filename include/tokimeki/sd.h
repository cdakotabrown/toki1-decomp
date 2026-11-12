#ifndef TOKIMEKI_SD_H
#define TOKIMEKI_SD_H

/* gSDInfo (0x8012DD80): the Konami sound driver state, one 80-byte record
 * (one global in o.bin).  gSDSerialInfo is its +4 word. */

typedef struct {
    unsigned int flags;              /* 0x00 */
    unsigned int serial_flags;       /* 0x04 (gSDSerialInfo) */
    int xa_start;           /* 0x08 */
    int xa_end;             /* 0x0C */
    int xa_next_start;      /* 0x10 */
    int xa_next_end;        /* 0x14 */
    int xa_resume;          /* 0x18 */
    short unk1C;         /* 0x1C (= the global serial_vol) */
    short unk1E;    /* 0x1E (= the global serial_vol_base) */
    short unk20;          /* 0x20 (= the global seq_vol_l) */
    short unk22;          /* 0x22 (= the global seq_vol_r) */
    short unk24;        /* 0x24 (= the global voice_vol_l) */
    short unk26;        /* 0x26 (= the global voice_vol_r) */
    short unk28;        /* 0x28 (= the global cur_seq_num) */
    short seq_no;             /* 0x2A (gSDSeqNo) */
    short unk2C;            /* 0x2C (= the global seq_req) */
    short unk2E;       /* 0x2E (= the global seq_fade_acc) */
    short unk30;      /* 0x30 (= the global seq_fade_step) */
    short unk32;       /* 0x32 (= the global seq_fade_lim) */
    short unk34;    /* 0x34 (= the global serial_fade_acc) */
    short unk36;   /* 0x36 (= the global serial_fade_step) */
    short unk38;    /* 0x38 (= the global serial_fade_lim) */
    unsigned short seq_bank;           /* 0x3A */
    short vab_ch[4];          /* 0x3C sd_vab_ch0..3 */
    unsigned char unk44;      /* 0x44 CdlSetmode parameter (200) (= the global serial_vol_arg) */
    unsigned char unk45[3];            /* 0x45 */
    unsigned char filter_file;         /* 0x48 CdlSetfilter parameter */
    unsigned char filter_chan;         /* 0x49 */
    unsigned char unk4A[2];            /* 0x4A */
    unsigned char xa_chan;             /* 0x4C */
    unsigned char xa_chan_next;        /* 0x4D */
    unsigned char xa_file;             /* 0x4E */
    unsigned char xa_file_next;        /* 0x4F */
} SD_INFO;

#endif
