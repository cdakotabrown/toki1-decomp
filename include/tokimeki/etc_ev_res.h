#ifndef TOKIMEKI_ETC_EV_RES_H
#define TOKIMEKI_ETC_EV_RES_H

/* ETC per-event resource records (etc_evNN_res, 0x8014EA90..0x8014F680).
 * Each event's init (etc_bgset_initNN / etc_ptrtab_setN / etc_rec_setN /
 * etc_tbl_initN) fills one record with addresses inside the loaded event
 * data; its etc_mainNN hands the fields on:
 *   palette_upload_enqueue(pal, 17, 1, 2, 0);
 *   bg_set6(bg[0..5]);
 *   date_object_set(cels, anim_tbl, hdr, scale);
 * date_object_set stores cels / anim_tbl / hdr into class_w[96..97]
 * (+0x0C / +0x10 / +0x34).  Every pointer is a 32-bit PS1 address cell
 * (int, never a C pointer), so the layout is the same on the PC port. */

typedef struct {
    int hdr;            /* 0x00 object header in the event data */
    int cels;           /* 0x04 cel table -> class_w.cels */
    int anim_tbl;       /* 0x08 animation table -> class_w.anim_tbl */
    short scale;        /* 0x0C copied from the event data */
    int pal;            /* 0x10 palette buffer (0x801B0000) */
    int bg[6];          /* 0x14 BG buffers 0x801B2000..0x801C6000 */
} ETC_EV_RES;

/* As ETC_EV_RES, plus the event data block base and the previous block
 * (written by the inits, read by no code). */
typedef struct {
    int hdr;            /* 0x00 */
    int cels;           /* 0x04 */
    int anim_tbl;       /* 0x08 */
    short scale;        /* 0x0C */
    int pal;            /* 0x10 */
    int blk;            /* 0x14 event data block holding hdr/cels/anim_tbl */
    int bg[6];          /* 0x18 */
    int blk_prev;       /* 0x30 the block 0x4000 below blk */
} ETC_EV_RES_BLK;

/* Events without a date object: palette + BG only. */
typedef struct {
    int pal;            /* 0x00 */
    int bg[6];          /* 0x04 */
} ETC_EV_RES_BG;

/* Events without BG: object + palette only. */
typedef struct {
    int hdr;            /* 0x00 */
    int cels;           /* 0x04 */
    int anim_tbl;       /* 0x08 */
    short scale;        /* 0x0C */
    int pal;            /* 0x10 */
} ETC_EV_RES_OBJ;

typedef char etc_ev_res_size_check[sizeof(ETC_EV_RES) == 44 ? 1 : -1];
typedef char etc_ev_res_blk_size_check[sizeof(ETC_EV_RES_BLK) == 52 ? 1 : -1];
typedef char etc_ev_res_bg_size_check[sizeof(ETC_EV_RES_BG) == 28 ? 1 : -1];
typedef char etc_ev_res_obj_size_check[sizeof(ETC_EV_RES_OBJ) == 20 ? 1 : -1];

#endif
