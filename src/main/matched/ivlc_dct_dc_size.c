/* Address: 0x800875C0 */
/* Size: 496 bytes (124 instructions) */
/* Region: Library (Psy-Q GCC 2.6.0, -O2, sched1 off) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -fno-schedule-insns --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u16 len;
    u16 size;
    u16 pad[2];
} DcCode;

extern s32 libpress_ivlc_block_idx;
extern u8 bs_curt_b3;
extern u32 bs_curt;
extern s32 dec_used;
extern u16 *libpress_ivlc_bs_read_ptr;
extern s32 libpress_ivlc_dc_pred_cr;
extern s32 libpress_ivlc_dc_pred_cb;
extern s32 libpress_ivlc_dc_pred_y;
extern DcCode DCC_DVLC[];
extern DcCode ivlc_dct_dc_size_tbl0[];

s32 ivlc_dct_dc_size(void)
{
    s32 diff;
    DcCode *p;
    s32 size;
    s32 half;
    u32 x;
    s32 y;
    u32 b;
    s32 n;
    s32 t;
    u32 c;

    diff = 0;
    if (libpress_ivlc_block_idx < 2) {
        p = &DCC_DVLC[bs_curt_b3];
    } else {
        p = &ivlc_dct_dc_size_tbl0[bs_curt_b3];
    }
    b = bs_curt;
    x = p->len;
    n = dec_used;
    do {
        do {
            x = b << x;
            bs_curt = x;
        } while (0);
    } while (0);
    y = p->len;
    y = y + n;
    dec_used = y;
    if (y & 0x10) {
        bs_curt = (*libpress_ivlc_bs_read_ptr++ << (dec_used = y & 0xF)) | x;
    }
    size = p->size;
    if (size != 0) {
        c = bs_curt;
        diff = c >> (32 - size);
        half = 1 << (size - 1);
        if (diff < half) {
            diff += 1 - (half << 1);
        }
        bs_curt = bs_curt << size;
        dec_used = dec_used + size;
        if (dec_used & 0x10) {
            bs_curt |= *libpress_ivlc_bs_read_ptr++ << (dec_used &= 0xF);
        }
    }
    switch (libpress_ivlc_block_idx) {
    case 0:
        t = diff + libpress_ivlc_dc_pred_cr;
        libpress_ivlc_dc_pred_cr = t;
        break;
    case 1:
        t = diff + libpress_ivlc_dc_pred_cb;
        libpress_ivlc_dc_pred_cb = t;
        break;
    default:
        t = diff + libpress_ivlc_dc_pred_y;
        libpress_ivlc_dc_pred_y = t;
        break;
    }
    do {
        do {
            diff = t;
        } while (0);
    } while (0);
    libpress_ivlc_block_idx = libpress_ivlc_block_idx + 1;
    if (libpress_ivlc_block_idx == 6) {
        libpress_ivlc_block_idx = 0;
    }
    return diff << 2;
}
