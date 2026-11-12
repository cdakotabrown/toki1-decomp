#ifndef TOKIMEKI_LIBGS_TYPES_H
#define TOKIMEKI_LIBGS_TYPES_H

/* Psy-Q libgs record layouts with Sony's field names, under project type
 * names (many matched files keep their own local MATRIX / GsDOBJ2 typedefs,
 * which Sony's names would collide with).  Used to spell fields of the
 * game's libgs-typed globals (coord_buf, taco_coord, taco_obj, ref_view,
 * pslt, fogp, Bg). */

typedef struct {
    s16 m[3][3];
    s32 t[3];
} LIBGS_MATRIX;                 /* MATRIX, 32 bytes */

typedef struct {
    u32 flg;
    LIBGS_MATRIX coord;
    LIBGS_MATRIX workm;
    u32 param;                  /* PS1 pointers: 32-bit cells on every host */
    u32 super;                  /* LIBGS_COORD2 * */
    u32 sub;                    /* LIBGS_COORD2 * */
} LIBGS_COORD2;                 /* GsCOORDINATE2, 80 bytes */

typedef struct {
    u32 attribute;
    u32 coord2;                 /* LIBGS_COORD2 * */
    u32 tmd;                    /* u32 * */
    u32 id;
} LIBGS_DOBJ2;                  /* GsDOBJ2, 16 bytes */

typedef struct {
    s32 vpx, vpy, vpz;
    s32 vrx, vry, vrz;
    s32 rz;
    u32 super;                  /* LIBGS_COORD2 * */
} LIBGS_RVIEW2;                 /* GsRVIEW2, 32 bytes */

typedef struct {
    s32 vx, vy, vz;
    u8 r, g, b;
} LIBGS_F_LIGHT;                /* GsF_LIGHT, 16 bytes */

typedef struct {
    s16 dqa;
    s32 dqb;
    u8 rfc, gfc, bfc;
} LIBGS_FOGPARAM;               /* GsFOGPARAM, 12 (16 aligned) bytes */

typedef struct {
    u32 attribute;
    s16 x, y;
    s16 w, h;
    s16 scrollx, scrolly;
    u8 r, g, b;
    u32 map;                    /* GsMAP * */
    s16 mx, my;
    s16 scalex, scaley;
    s32 rotate;
} LIBGS_BG;                     /* GsBG, 36 bytes */

/* The game's globals live at their PS1 addresses in PS1 layout on every
 * host (the PC port too), so a pointer field is a 32-bit PS1 address cell,
 * never a host pointer.  These fail to compile if a layout drifts. */
typedef char libgs_matrix_size_check[sizeof(LIBGS_MATRIX) == 32 ? 1 : -1];
typedef char libgs_coord2_size_check[sizeof(LIBGS_COORD2) == 80 ? 1 : -1];
typedef char libgs_dobj2_size_check[sizeof(LIBGS_DOBJ2) == 16 ? 1 : -1];
typedef char libgs_rview2_size_check[sizeof(LIBGS_RVIEW2) == 32 ? 1 : -1];
typedef char libgs_f_light_size_check[sizeof(LIBGS_F_LIGHT) == 16 ? 1 : -1];
typedef char libgs_bg_size_check[sizeof(LIBGS_BG) == 36 ? 1 : -1];

#endif
