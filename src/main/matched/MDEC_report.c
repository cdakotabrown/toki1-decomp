/* Function: MDEC_report */
/* Address: 0x80086E9C */
/* Size: 208 bytes (52 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc263p */
/* @CompileFlags: -O2 -fsched-old-blockage -fno-schedule-insns2 --fill-earliest */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: maspsx --aspsx-version=2.67 */
/* @PostProc: swap_instructions --offset1 0x0 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x90 --offset2 0xa0 */
/* @PostProc: swap_instructions --offset1 0x94 --offset2 0xa8 */
/* @PostProc: swap_instructions --offset1 0x98 --offset2 0xa0 */
/* @PostProc: swap_instructions --offset1 0x9c --offset2 0xa0 */
/* @PostProc: swap_instructions --offset1 0xa4 --offset2 0xa8 */
/* @PostProc: swap_instructions --offset1 0xa8 --offset2 0xac */
/* @PostProc: swap_instructions --offset1 0xac --offset2 0xb0 */
#include "types.h"

extern void printf(const char *fmt, ...);

extern volatile u32 *libpress_mdec_ctrl_reg_ptr;
extern volatile u32 *libpress_dma0_chcr_reg_ptr;
extern volatile u32 *libpress_dma1_chcr_reg_ptr;
extern volatile u32 *libpress_dma1_madr_reg_ptr;
extern volatile u32 *libpress_dma0_madr_reg_ptr;

s32 MDEC_report(void)
{
    u32 val;

    val = *libpress_mdec_ctrl_reg_ptr;

    printf("\t DMA=(%d,%d), ADDR=(0x%08x->0x%08x)\n",
        (*libpress_dma0_chcr_reg_ptr >> 24) & 1,
        (*libpress_dma1_chcr_reg_ptr >> 24) & 1,
        *libpress_dma0_madr_reg_ptr,
        *libpress_dma1_madr_reg_ptr);

    printf("\t FIFO=(%d,%d),BUSY=%d,DREQ=(%d,%d),RGB24=%d,STP=%d\n",
        ~val >> 31,
        (val >> 30) & 1,
        (val >> 29) & 1,
        (val >> 28) & 1,
        (val >> 27) & 1,
        (val >> 25) & 1,
        (val >> 23) & 1);

    return 0;
}
