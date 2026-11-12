#ifndef TOKIMEKI_ENDING_GIRL_RES_H
#define TOKIMEKI_ENDING_GIRL_RES_H

/* ENDING bust-up resource records (ending_girl_res[15], 0x8013C650).
 * Slot k is filled by girl k's init (ending_girl_init0, ending_girl3_set,
 * ...; [12] default, [13]/[14] alternate forms of girls 1 and 6) and
 * ending_sprites_init hands slot now_girl's hdr/img/anim_tbl/aux to
 * ending_bustup2_set; slot 0's palette and 12 tiles are uploaded there.
 * Every pointer is a 32-bit PS1 address cell (u32, never a C pointer), so
 * the layout is the same on the PC port. */

typedef struct {
    unsigned int hdr;       /* 0x00 bust-up header in the loaded file */
    unsigned int img;       /* 0x04 cel table */
    unsigned int anim_tbl;  /* 0x08 animation table */
    short aux;              /* 0x0C copied from the loaded file */
    unsigned int pal;       /* 0x10 palette buffer */
    unsigned int tile[12];  /* 0x14 image tiles */
} ENDING_GIRL_RES;

typedef char ending_girl_res_size_check[sizeof(ENDING_GIRL_RES) == 68 ? 1 : -1];

#endif
