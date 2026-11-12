#ifndef TOKIMEKI_ETC_CG_RES_H
#define TOKIMEKI_ETC_CG_RES_H

/* Resource block of a date picture with a summer / other-season set:
 * shougatu_girl1_res (0x80145470).  shougatu_res_set writes the 18 words;
 * shougatu_date_sprite_init2 uploads pal[0] and tile[0] when now_month is
 * 6..9, else pal[1] and tile[1], then date_object_set(img, anim_tbl, hdr,
 * aux).  Every pointer is a 32-bit PS1 address cell (an int, never a
 * C pointer), so the layout is the same on the PC port. */

typedef struct {
    int hdr;           /* 0x00 object header in the loaded file */
    int img;           /* 0x04 cel table */
    int anim_tbl;      /* 0x08 animation table */
    short aux;                  /* 0x0C copied from the loaded file */
    int pal[2];        /* 0x10 palette buffers (summer, other) */
    int tile[2][6];    /* 0x18 image tiles (summer, other) */
} ETC_CG_RES;

typedef char etc_cg_res_size_check[sizeof(ETC_CG_RES) == 72 ? 1 : -1];

#endif
