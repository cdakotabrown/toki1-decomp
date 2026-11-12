/* Function: _spu_r (_spu_r) */
/* Address: 0x80095EB4 */
/* Size: 276 bytes (69 instructions) */
/* Region: Library (0x80086000+) */
/* - `@CompileEnv: IDO52_AS1_KEEP_ANDI=all` vetoes the as1 fold. */
/* @CompileEnv: IDO52_AS1_KEEP_ANDI=all IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_FORCE_WEB_COLOR=33:a2,83:a3,52:a1,45:v1 IDO52_UGEN_TEMP_REG=1:v1,5:a0,6:t0,7:t1,10:t4,8:t2,9:t3,16:t9,17:t0 IDO52_UGEN_TEMP_REG_INPLACE=1,5 IDO52_AS1_NO_COPY_PROP=0 IDO52_AS1_TIE_REJECT=149,150 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern void *_spu_RXX;
extern s32 *libspu_dma4_madr_reg_ptr;
extern s32 *libspu_dma4_bcr_reg_ptr;
extern s32 *libspu_dma4_chcr_reg_ptr;
extern s32 *libspu_dma_dpcr_reg_ptr;
extern s32 *libspu_dma_dicr_reg_ptr;
extern u16 spu_tsa;
extern s32 spu_dma_mode;
extern s32 spu_transfer_startaddr;
extern u32 spu_transfer_size;
extern s32 spu_transfer_time;

extern void _spu_setDelayR(void);

void _spu_r(s32 arg0, u32 arg1) {
    volatile u16 *base;
    u16 reg;
    s32 sectors;
    s32 *p;
    s32 total;
    s32 *q;

    base = (volatile u16 *)_spu_RXX;

    reg = base[0xD5];
    reg = reg & 0xFFCF;
    base[0xD5] = reg;
    reg = spu_tsa & 0xFFFF;
    base[0xD3] = reg;
    reg = base[0xD5];
    reg = reg & 0xFFCF;
    reg = reg | 0x30;
    base[0xD5] = reg;
    _spu_setDelayR();
    spu_transfer_startaddr = arg0;
    spu_transfer_size = arg1;
    if (arg1 & 0x3F) {
        sectors = 1;
    } else {
        sectors = 0;
    }
    total = (s32)(arg1 >> 6) + sectors;
    p = libspu_dma_dicr_reg_ptr;
    q = libspu_dma_dpcr_reg_ptr;
    *p |= 0x900000;
    *q |= 0x80000;
    *libspu_dma4_madr_reg_ptr = arg0;
    *libspu_dma4_bcr_reg_ptr = (total << 16) | 0x10;
    spu_dma_mode = 1;
    *libspu_dma4_chcr_reg_ptr = 0x01000200;
    spu_transfer_time = total;
}
