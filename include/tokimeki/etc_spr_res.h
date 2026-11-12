#ifndef TOKIMEKI_ETC_SPR_RES_H
#define TOKIMEKI_ETC_SPR_RES_H

/* ETC kind-8 (sprite mode) event records: etc_spr46_res (0x8014EFE0,
 * etc_tbl_init3) and etc_spr0e_res (0x8014F104, etc_tbl_init9).
 * etc_sprite_initNN picks the summer set when etc_rec_month is 6..9, else
 * the other set.  Every pointer is a 32-bit PS1 address cell (an int,
 * never a C pointer), so the layout is the same on the PC port.  scale is
 * a halfword the init copies; etc_sprite_init0e also reads it as a word. */

typedef struct {
    int hdr;           /* 0x00 object header in the event data */
    int cels;          /* 0x04 cel table */
    int anim_tbl;      /* 0x08 animation table */
    union {
        short h;                /* 0x0C */
        int w;                  /* 0x0C word view */
    } scale;
    int pal_summer;    /* 0x10 */
    int pal_other;     /* 0x14 */
    int bg_summer[6];  /* 0x18 */
    int bg_other[6];   /* 0x30 */
} ETC_SPR_RES;

typedef char etc_spr_res_size_check[sizeof(ETC_SPR_RES) == 72 ? 1 : -1];

#endif
