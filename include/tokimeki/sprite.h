#ifndef TOKIMEKI_SPRITE_H
#define TOKIMEKI_SPRITE_H

/* Sprites (0x80129CB0): 64 sprite records with libgs GsSPRITE's layout and
 * field names (the types are the ones the matched C reads them with). */
typedef struct {
    u32 attribute;              /* 0x00 bit 31: hidden */
    s16 x, y, w, h, tpage;      /* 0x04 */
    u8  u, v;                   /* 0x0E */
    s16 cx, cy;                 /* 0x10 */
    u8  r, g, b;                /* 0x14 */
    s16 mx, my, scalex, scaley; /* 0x18 */
    u32 rotate;                 /* 0x20 */
} SPRITE_REC;

#define SPRITES_MAX 64


#endif
