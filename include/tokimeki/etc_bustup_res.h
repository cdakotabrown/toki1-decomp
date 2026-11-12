#ifndef TOKIMEKI_ETC_BUSTUP_RES_H
#define TOKIMEKI_ETC_BUSTUP_RES_H

/* ETC bust-up buffer records: etc_bustup_buf_res (0x8014F680, written by
 * bustup_buf_ptrs_init) and etc_bustup2_buf_res (0x8014F6AC, written by
 * bustup2_buf_ptrs_init): three pointers into the loaded file, a halfword
 * and seven buffer pointers (0x80197000..0x801AD000).  Nothing in ETC reads
 * them.  Every pointer is a 32-bit PS1 address cell (an int, never a
 * C pointer), so the layout is the same on the PC port. */

typedef struct {
    int hdr;       /* 0x00 */
    int img;       /* 0x04 */
    int anim_tbl;  /* 0x08 */
    short aux;              /* 0x0C copied from the loaded file */
    int buf[7];    /* 0x10 buffers 0x80197000..0x801AD000 */
} ETC_BUSTUP_RES;

typedef char etc_bustup_res_size_check[sizeof(ETC_BUSTUP_RES) == 44 ? 1 : -1];

#endif
