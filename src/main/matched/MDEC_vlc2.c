/* @SymbolStart: 0x80087200 */
/* @SymbolSize: 960 */
/* Function: MDEC_vlc2 */
/* Address: 0x80087210 */
/* Size: 944 bytes (236 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 --fill-load-delay -fno-cse-follow-jumps --fill-epilogue --fill-branch-store --fill-ptr-bump-down --fill-jal-store */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions 0x0 0x18 */
/* @PostProc: swap_instructions 0x4 0x1c */
/* @PostProc: swap_instructions 0x8 0x20 */
/* @PostProc: swap_instructions 0xc 0x24 */
/* @PostProc: swap_instructions 0x10 0x18 */
/* @PostProc: swap_instructions 0x14 0x28 */
/* @PostProc: swap_instructions 0x18 0x2c */
/* @PostProc: swap_instructions 0x1c 0x30 */
/* @PostProc: swap_instructions 0x20 0x28 */
/* @PostProc: swap_instructions 0x24 0x34 */
/* @PostProc: swap_instructions 0x2c 0x34 */
/* @PostProc: swap_instructions 0x30 0x34 */
/* @PostProc: swap_instructions 0x94 0x98 */
/* @PostProc: swap_instructions 0x9c 0xa0 */
/* @PostProc: swap_instructions 0xac 0xb0 */
/* @PostProc: swap_instructions 0xf0 0xf4 */
/* @PostProc: swap_instructions 0xf4 0xf8 */
/* @PostProc: swap_instructions 0x114 0x118 */
/* @PostProc: swap_instructions 0x118 0x11c */
/* @PostProc: swap_instructions 0x120 0x12c */
/* @PostProc: swap_instructions 0x124 0x12c */
/* @PostProc: swap_instructions 0x128 0x130 */
/* @PostProc: swap_instructions 0x170 0x180 */
/* @PostProc: swap_instructions 0x174 0x184 */
/* @PostProc: swap_instructions 0x188 0x18c */
/* @PostProc: swap_instructions 0x18c 0x190 */
/* @PostProc: swap_instructions 0x1dc 0x1e0 */
/* @PostProc: swap_instructions 0x1e0 0x1e4 */
/* @PostProc: swap_instructions 0x1e8 0x1ec */
/* @PostProc: swap_instructions 0x1ec 0x1f0 */
/* @PostProc: swap_instructions 0x1f4 0x200 */
/* @PostProc: swap_instructions 0x1f8 0x200 */
/* @PostProc: swap_instructions 0x1fc 0x204 */
/* @PostProc: swap_instructions 0x234 0x238 */
/* @PostProc: swap_instructions 0x238 0x23c */
/* @PostProc: swap_instructions 0x250 0x254 */
/* @PostProc: swap_instructions 0x2b4 0x2b8 */
/* @PostProc: swap_instructions 0x2f4 0x2f8 */
/* @PostProc: swap_instructions 0x350 0x358 */
/* @PostProc: swap_instructions 0x354 0x358 */
/* @PostProc: swap_instructions 0x358 0x35c */
/* @PostProc: swap_instructions 0x35c 0x360 */
#include "types.h"

extern s32 libpress_ivlc_dc_pred_y;
extern s32 libpress_ivlc_dc_pred_cr;
extern s32 libpress_ivlc_dc_pred_cb;
extern s32 libpress_ivlc_block_idx;
extern s32 dec_used;
extern u16 *libpress_ivlc_bs_read_ptr;
extern u32 bs_curt;

typedef struct {
    u16 len;
    u16 v[3];
} Vlc;

extern Vlc CF_DVLC[];
extern Vlc CF2_DVLC[];

extern void printf(char *);
extern s32 ivlc_dct_dc_size(void);
extern char libpress_mdec_vlc_bad_id_msg[];

#define SKIP(n) { \
    bs_curt = bs_curt << (n); \
    dec_used = dec_used + (n); \
    if (dec_used & 0x10) { \
        bs_curt |= *libpress_ivlc_bs_read_ptr++ << (dec_used &= 0xF); \
    } }

s32 MDEC_vlc2(u16 *src, u16 *dst)
{
    u16 hdr0;
    u16 dcq;
    Vlc *entry;
    s32 v1;
    u16 v0;
    u32 w;
    u16 *t;
    u32 c;
    s32 mode;
    s32 bp;
    u32 i2;
    u32 tb;
    s32 r;
    u32 dcv;

    libpress_ivlc_dc_pred_y = 0;
    libpress_ivlc_dc_pred_cr = 0;
    libpress_ivlc_dc_pred_cb = 0;
    libpress_ivlc_block_idx = 0;
    v0 = *src;
    hdr0 = *(volatile u16 *)src;
    *dst = v0;
    dst[1] = src[1];
    src++;
    __asm__ volatile ("" : : "r" (src));
    src++;
    dst++;
    mode = src[1];
    dcq = *src;
    __asm__ volatile ("" : : "r" (dst));
    dst++;
    src++;
    __asm__ volatile ("" : : "r" (src));
    src++;
    if (mode != 3) {
        printf(libpress_mdec_vlc_bad_id_msg);
        return 0;
    }
    {
        u16 *p = src;
        u32 lo;
        u32 hi;
        dec_used = 0;
        hi = *p++;
        lo = *p++;
        libpress_ivlc_bs_read_ptr = p;
        bs_curt = (hi << 16) | lo;
    }
    r = ivlc_dct_dc_size();
    t = dst;
    dst++;
    v1 = r;
    c = dcq << 10;
    if (v1 >= 0) {
dcpos:
        dcv = c | v1;
    } else {
dcneg:
        dcv = c | (v1 + 1024);
    }
    *t = dcv;

loop:
    w = bs_curt;
    entry = &CF_DVLC[w >> 19];
    if (entry->v[0] != 0) {
        bs_curt = w = w << entry->len;
        dec_used = dec_used + entry->len;
        if (dec_used & 0x10) {
            bs_curt = (*libpress_ivlc_bs_read_ptr++ << (dec_used &= 0xF)) | w;
        }
    } else {
        i2 = w >> 15;
        bp = dec_used;
        tb = (u32)CF2_DVLC;
        i2 <<= 3;
        bs_curt = w = w << 8;
        entry = (Vlc *)(tb + i2);
        bp += 8;
        dec_used = bp;
        if (dec_used & 0x10) {
            bs_curt = (*libpress_ivlc_bs_read_ptr++ << (dec_used &= 0xF)) | w;
        }
        SKIP(entry->len);
    }

    v1 = (*dst = entry->v[0]);
    if (v1 == 0x7C1F) {
        goto esc;
    }
    if (v1 == 0xFE00) {
dc:
        v1 = bs_curt >> 22;
        dst++;
        if (v1 == 0x3FF) {
            goto pad;
        }
        r = ivlc_dct_dc_size();
        t = dst;
        dst++;
        v1 = r;
        c = dcq << 10;
        if (v1 >= 0) {
            goto dcpos;
        }
        goto dcneg;
    }
    v0 = entry->v[1];
    *++dst = v0;
    v1 = v0;
    if (v1 == 0x7C1F) {
        goto esc;
    }
    if (v1 < 0x7C20) {
        if (v1 == 0) {
            goto loop;
        }
    } else if (v1 == 0xFE00) {
        goto dc;
    }
    v0 = entry->v[2];
    *++dst = v0;
    v1 = v0;
    if (v1 == 0x7C1F) {
        goto esc;
    }
    if (v1 < 0x7C20) {
        if (v1 == 0) {
            goto loop;
        }
    } else if (v1 == 0xFE00) {
        goto dc;
    }
    dst++;
    goto loop;
esc:
    *dst = bs_curt >> 16;
    bs_curt = bs_curt << 16;
    bs_curt |= *libpress_ivlc_bs_read_ptr++ << dec_used;
    dst++;
    goto loop;
pad:
    for (v1 = 0; v1 < 64; v1++) {
        *dst++ = 0xFE00;
    }
    return hdr0 + 1;
}
