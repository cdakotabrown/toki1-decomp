/* Function: MDEC_in (MDEC_reset) */
/* Address: 0x80086C3C */
/* Size: 148 bytes (37 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc272 */
/* @CompileFlags: -O2 --fill-epilogue */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x70 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x88 */
/* @PostProc: patch_insn_reg_field --patch 0x48:rt:v1:v0 --patch 0x4c:rt:v1:v0 --patch 0x4c:rs:v1:v0 --patch 0x54:rs:v1:v0 */
#include "types.h"

extern u32 *libpress_dma_dpcr_reg_ptr;
extern u32 *libpress_dma0_madr_reg_ptr;
extern u32 *libpress_dma0_bcr_reg_ptr;
extern u32 *libpress_mdec_cmd_reg_ptr;
extern u32 *libpress_dma0_chcr_reg_ptr;

extern void MDEC_in_sync(s32 *arg0);

void MDEC_in(s32 *arg0, u32 arg1) {
    u32 *ptr;
    u32 val;

    MDEC_in_sync(arg0);

    ptr = libpress_dma_dpcr_reg_ptr;
    arg1 = arg1 >> 5;
    val = *ptr;
    arg1 = arg1 << 16;
    val = val | 0x88;
    *ptr = val;

    ptr = libpress_dma0_madr_reg_ptr;
    val = (u32)(arg0 + 1);
    *ptr = val;

    ptr = libpress_dma0_bcr_reg_ptr;
    arg1 = arg1 | 0x20;
    *ptr = arg1;

    ptr = libpress_mdec_cmd_reg_ptr;
    val = *arg0;
    *ptr = val;

    ptr = libpress_dma0_chcr_reg_ptr;
    val = 0x01000201;
    *ptr = val;
}
