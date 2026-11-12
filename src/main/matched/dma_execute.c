/* Address: 0x80088C74 */
/* Size: 412 bytes (103 instructions) */
/* Region: Library (0x80086000-0x800BFFFF) */
/* Compiler: Psy-Q GCC 2.6.3 (cc1-psx-26) + ASPSX (NOT IDO). */
/* @Compiler: gcc263 */
/* @CompileFlags: -O2 -fno-schedule-insns --fill-epilogue --load-dest-temp --fill-ptr-bump-down --fill-load-delay */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x3c */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x108 --offset2 0x110 */
/* @PostProc: swap_instructions --offset1 0x10c --offset2 0x11c */
/* @PostProc: swap_instructions --offset1 0x110 --offset2 0x120 */
/* @PostProc: swap_instructions --offset1 0x114 --offset2 0x118 */
/* @PostProc: swap_instructions --offset1 0x118 --offset2 0x12c */
/* @PostProc: swap_instructions --offset1 0x11c --offset2 0x124 */
/* @PostProc: swap_instructions --offset1 0x120 --offset2 0x134 */
/* @PostProc: swap_instructions --offset1 0x124 --offset2 0x128 */
/* @PostProc: swap_instructions --offset1 0x128 --offset2 0x138 */
/* @PostProc: swap_instructions --offset1 0x12c --offset2 0x130 */
/* @PostProc: swap_instructions --offset1 0x130 --offset2 0x13c */
/* @PostProc: swap_instructions --offset1 0x134 --offset2 0x140 */
/* @PostProc: swap_instructions --offset1 0x138 --offset2 0x140 */
/* @PostProc: swap_instructions --offset1 0x13c --offset2 0x148 */
/* @PostProc: swap_instructions --offset1 0x144 --offset2 0x148 */
/* @PostProc: swap_instructions --offset1 0x14c --offset2 0x154 */
/* @PostProc: swap_instructions --offset1 0x150 --offset2 0x154 */
/* @PostProc: swap_instructions --offset1 0x168 --offset2 0x174 */
/* @PostProc: swap_instructions --offset1 0x16c --offset2 0x190 */
/* @PostProc: swap_instructions --offset1 0x170 --offset2 0x18c */
/* @PostProc: swap_instructions --offset1 0x174 --offset2 0x188 */
/* @PostProc: swap_instructions --offset1 0x178 --offset2 0x184 */
/* @PostProc: swap_instructions --offset1 0x17c --offset2 0x180 */
/* @PostProc: swap_instructions --offset1 0x190 --offset2 0x198 */
#include "types.h"

extern u32 *libcd_dma_exec_dicr_ptr;
extern u32 *libcd_dma_exec_dpcr_ptr;
extern u8 dma_execute_tbl0[];
extern void printf(u8 *fmt, u32 a);

struct Dw { u32 w; };

void dma_execute(s32 ch, u32 madr, u32 bcrHi, u32 bcrLo, u32 chcr,
                   s32 mode, u32 prio)
{
    volatile u32 *p;
    volatile u32 dummy;
    s32 i;
    struct Dw *q;
    u32 val;
    u32 *r;
    u32 b;
    u32 k;
    u32 off;

    i = 0;
    while (((volatile u32 *)(0x1F801080 + (ch << 4)))[2] & 0x01000000) {
        if (i == 0x10000) {
            printf(dma_execute_tbl0,
                          ((volatile u32 *)(0x1F801080 + (ch << 4)))[2]);
            break;
        }
        i++;
    }
    if (mode == 1) {

        do { q = (struct Dw *)libcd_dma_exec_dicr_ptr; } while (0);
        val = q->w;
        mode = 0x10000;
        mode <<= ch;
        prio <<= ch;
        mode |= prio;
        val &= 0x00FFFFFF;
        val |= mode;
    } else {
        mode = 1 << (ch + 16);

        do { q = (struct Dw *)libcd_dma_exec_dicr_ptr; } while (0);
        val = q->w;
        val &= 0x00FFFFFF;
        val &= ~mode;
    }
    r = libcd_dma_exec_dpcr_ptr;
    b = 0x1F801080;
    q->w = val;
    dummy = *libcd_dma_exec_dicr_ptr;
    off = ch << 4;
    val = ch * 4;
    val += 3;
    k = 1 << val;
    val = *r;
    val |= k;
    *r = val;
    p = (volatile u32 *)(b + off);
    *p++ = madr;
    *p++ = (bcrHi << 16) | bcrLo;
    *p = chcr;
    dummy = *p;
}
