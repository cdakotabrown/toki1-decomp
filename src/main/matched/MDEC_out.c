/* Address: 0x80086CD0 */
/* Size: 140 bytes (35 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc272 */
/* @CompileFlags: -O2 --fill-epilogue */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x80 */
/* @PostProc: patch_insn_reg_field --patch 0x1c:rt:v0:v1 --patch 0x20:rt:v0:v1 --patch 0x20:rs:v0:v1 --patch 0x28:rt:v1:v0 --patch 0x28:rs:v0:v1 --patch 0x30:rt:v1:v0 --patch 0x30:rs:v1:v0 --patch 0x34:rt:v1:v0 --patch 0x34:rs:v0:v1 --patch 0x50:rt:v0:v1 --patch 0x68:rt:v0:v1 --patch 0x68:rs:v0:v1 --patch 0x6c:rt:v1:v0 --patch 0x70:rt:v1:v0 --patch 0x70:rs:v1:v0 --patch 0x74:rt:v1:v0 --patch 0x74:rs:v0:v1 */
#include "types.h"

extern u32 *libpress_dma_dpcr_reg_ptr;
extern u32 *libpress_dma1_chcr_reg_ptr;
extern u32 *libpress_dma1_madr_reg_ptr;
extern u32 *libpress_dma1_bcr_reg_ptr;
extern void MDEC_out_sync(void);

void MDEC_out(u32 arg0, u32 arg1) {
    u32 *ptr;
    u32 val;

    MDEC_out_sync();

    ptr = libpress_dma_dpcr_reg_ptr;
    arg1 = arg1 >> 5;
    val = *ptr;
    arg1 = arg1 << 16;
    val = val | 0x88;
    *ptr = val;

    ptr = libpress_dma1_chcr_reg_ptr;
    arg1 = arg1 | 0x20;
    *ptr = 0;

    ptr = libpress_dma1_madr_reg_ptr;
    *ptr = arg0;

    ptr = libpress_dma1_bcr_reg_ptr;
    *ptr = arg1;

    ptr = libpress_dma1_chcr_reg_ptr;
    val = 0x01000200;
    *ptr = val;
}
