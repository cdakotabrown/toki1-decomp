/* Address: 0x80095FC8 */
/* Size: 180 bytes (45 instructions) */
/* Region: Library (0x80086000+) */
/* Compiler: IDO 5.2 (CONFIRMED -- t6..t9 temp pool, `or` moves converted by */
/* @CompileEnv: IDO52_AS1_KEEP_ANDI=all IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_FORCE_WEB_COLOR=5:t2,10:t1,15:t0,23:v1,32:a3 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions 0x8 0x1c */
/* @PostProc: swap_instructions 0xc 0x10 */
/* @PostProc: swap_instructions 0x10 0x1c */
/* @PostProc: swap_instructions 0x14 0x1c */
/* @PostProc: swap_instructions 0x18 0x1c */
/* @PostProc: swap_instructions 0x1c 0x20 */
/* @PostProc: swap_instructions 0x20 0x2c */
/* @PostProc: swap_instructions 0x24 0x2c */
/* @PostProc: swap_instructions 0x28 0x38 */
/* @PostProc: swap_instructions 0x30 0x38 */
/* @PostProc: swap_instructions 0x34 0x38 */
/* @PostProc: swap_instructions 0x9c 0xa0 */
#include "types.h"

extern u8 * volatile _spu_RXX;
extern u32 *libspu_dma4_madr_reg_ptr;
extern u32 *libspu_dma4_bcr_reg_ptr;
extern u32 *libspu_dma4_chcr_reg_ptr;
extern u32 * volatile libspu_dma_dpcr_reg_ptr;
extern u32 * volatile libspu_dma_dicr_reg_ptr;
extern u32 * volatile libspu_dev_delay_reg_ptr;
extern s32  spu_dma_mode;

void _spu_r_(u32 a, s32 b, u32 c) {
    u8 *p = _spu_RXX;
    u32 *r80 = libspu_dma_dpcr_reg_ptr;
    u32 *r84 = libspu_dma_dicr_reg_ptr;
    u32 *r88 = libspu_dev_delay_reg_ptr;
    u32 h;
    u32 w;

    *(u16 *)(p + 422) = b;
    h = *(u16 *)(p + 426);
    h |= 0x30;
    h &= 0xFFFF;
    *(u16 *)(p + 426) = h;
    w = *r88;
    w &= 0xF0FFFFFF;
    w |= 0x22000000;
    *r88 = w;
    *r84 = *r84 | 0x00900000;
    *r80 = *r80 | 0x00080000;
    *libspu_dma4_madr_reg_ptr = a;
    *libspu_dma4_bcr_reg_ptr = (c << 16) | 0x10;
    spu_dma_mode = 1;
    *libspu_dma4_chcr_reg_ptr = 0x01000200;
}
