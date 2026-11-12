#ifndef TOKIMEKI_TT_BOX_H
#define TOKIMEKI_TT_BOX_H

/* TT hit boxes (tt_hit_box_tbl[49] 0x80155B50, tt_target_box_tbl[5]
 * 0x80155C78, tt_cell_box_tbl[35] 0x80155C98).  Six bytes each: the
 * top-left corner and the size.  tt_box_overlap_check(p1, box, p3, box)
 * reads two of them; the shot / camera / match inits store pointers to
 * single records (&tbl[n]) into object fields. */

typedef struct {
    short x;            /* 0x00 */
    short y;            /* 0x02 */
    unsigned char w;    /* 0x04 */
    unsigned char h;    /* 0x05 */
} TT_BOX;

typedef char tt_box_size_check[sizeof(TT_BOX) == 6 ? 1 : -1];

#endif
