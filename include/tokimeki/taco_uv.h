#ifndef TOKIMEKI_TACO_UV_H
#define TOKIMEKI_TACO_UV_H

/* TACO font TACO_UV pair (taco_glyph_uv_tbl[40], 0x8015F5C0): the (u, v) byte
 * pair of one glyph on a 12-px grid; taco_glyph_disp adds u + 128 into
 * setUV4.  taco_glyph_disp.c carries its own file-local copy of this
 * typedef; drop it when this header is included. */

typedef struct {
    unsigned char u;    /* 0x00 */
    unsigned char v;    /* 0x01 */
} TACO_UV;

typedef char taco_uv_size_check[sizeof(TACO_UV) == 2 ? 1 : -1];

#endif
