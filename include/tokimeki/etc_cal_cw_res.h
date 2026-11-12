#ifndef TOKIMEKI_ETC_CAL_CW_RES_H
#define TOKIMEKI_ETC_CAL_CW_RES_H

/* ETC calendar-cw record (etc_cal_cw_res, 0x80150050): etc_buf_init stores
 * hdr / img / anim_tbl (0x8019DCFC / DD00 / DDC0) and an aux halfword read
 * from 0x8019DDF8; etc_cal_cw_init copies them into cal_cw_hdr / img /
 * anim_tbl / aux14.  Every pointer is a 32-bit PS1 address cell (an
 * int, never a C pointer), so the layout is the same on the PC port. */

typedef struct {
    int hdr;       /* 0x00 */
    int img;       /* 0x04 */
    int anim_tbl;  /* 0x08 */
    short aux;              /* 0x0C loaded data */
    short pad_0e;           /* 0x0E */
} ETC_CAL_CW_RES;

typedef char etc_cal_cw_res_size_check[sizeof(ETC_CAL_CW_RES) == 16 ? 1 : -1];

#endif
