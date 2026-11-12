#ifndef TOKIMEKI_CLASS_W_H
#define TOKIMEKI_CLASS_W_H

/* class_w (0x801271B0): the object table, 160 slots of 68 bytes.  The layout
 * below is the sprite-slot view that class_clear_n and sprite_slot_disp use;
 * object3d_init / object3d_sort overlay the same memory as 136-byte pairs of
 * slots with a different layout.  unkNN fields are used but not yet named. */

/* a 16.16 fixed-point coordinate: fix is the whole word, half[1] the
 * integer part */
typedef union {
    s32 fix;
    s16 half[2];
} CLASS_FIX;

typedef struct {
    u8  active;     /* 0x00 */
    u8  prio;       /* 0x01 sort-table priority passed to GsSortSprite */
    u8  anim_flag;  /* 0x02 single-cel / playing / loop / ping-pong / reverse */
    u8  disp_flag;  /* 0x03 0x80 visible, rate gate, brightness, move, flip */
    u8  cel_mode;   /* 0x04 cel format: 8 = 8-bit CLUT cels */
    u8  tpage_sel;  /* 0x05 texture-page selector */
    u8  rate;       /* 0x06 */
    u8  bright;     /* 0x07 default 128 */
    s16 tick;       /* 0x08 */
    s16 unk0A;      /* 0x0A */
    u32 cels;       /* 0x0C image cel table (PS1 pointer cell) */
    u32 anim_tbl;   /* 0x10 animation table (PS1 pointer cell) */
    s16 unk14;      /* 0x14 */
    s16 anim;       /* 0x16 */
    s16 frame;      /* 0x18 */
    s16 scale_x;    /* 0x1A default 4096 */
    s16 scale_y;    /* 0x1C default 4096 */
    s16 unk1E;      /* 0x1E */
    s32 timing;     /* 0x20 */
    CLASS_FIX x;    /* 0x24 */
    CLASS_FIX y;    /* 0x28 */
    s32 vx;         /* 0x2C */
    s32 vy;         /* 0x30 */
    u32 hdr;        /* 0x34 (PS1 pointer cell) */
    u32 attribute;  /* 0x38 GsSPRITE attribute word */
    u32 unk3C;      /* 0x3C */
    u8  r;          /* 0x40 r/g/b override bright when any is nonzero */
    u8  g;          /* 0x41 */
    u8  b;          /* 0x42 */
    u8  clut;       /* 0x43 */
} CLASS_W;

#define CLASS_W_MAX 160

/* class_w is PS1-layout memory on every host (the PC port links it at its PS1
 * address), so pointer fields are 32-bit PS1 address cells.  Fails to compile
 * if the layout drifts from the console's 68-byte slot. */
typedef char class_w_size_check[sizeof(CLASS_W) == 68 ? 1 : -1];

#endif
