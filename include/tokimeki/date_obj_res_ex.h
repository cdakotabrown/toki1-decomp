#ifndef TOKIMEKI_DATE_OBJ_RES_EX_H
#define TOKIMEKI_DATE_OBJ_RES_EX_H

/* Object resource block of a DATE-style scene (date_ev11_res 0x8015EA68,
 * shougatu_ev06_res 0x80146570, geko_ptc_res 0x801462C0): hdr / img /
 * anim_tbl / aux go to date_object_set, pal is uploaded as clut 17 and
 * pal2 as clut 18, tile[6] go to date_sprite_init, ext_tile is loaded with
 * load_image_rect.  Every pointer is a 32-bit PS1 address cell (an
 * int, never a C pointer), so the layout is the same on the PC port. */

typedef struct {
    int hdr;       /* 0x00 object header in the loaded file */
    int img;       /* 0x04 cel table */
    int anim_tbl;  /* 0x08 animation table */
    short aux;              /* 0x0C copied from the loaded file */
    int pal;       /* 0x10 palette buffer (0x801B0000) */
    int pal2;      /* 0x14 second palette (clut 18) */
    int tile[6];   /* 0x18 date_sprite_init tiles */
    int ext_tile;  /* 0x30 extra tile image */
} DATE_OBJ_RES_EX;

/* GEKO's particle-scene block: identical layout. */
typedef DATE_OBJ_RES_EX GEKO_OBJ_RES_EX;

typedef char date_obj_res_ex_size_check[sizeof(DATE_OBJ_RES_EX) == 52 ? 1 : -1];

#endif
