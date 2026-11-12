/* Function: SD_BranchCTRL */
/* Address: 0x80088C74 */
/* Size: 412 bytes (103 instructions) */
/* Region: Library (0x80086000+) */
/* CONFIRMED UNMATCHABLE FROM C (2026-03-24, re-investigated 2026-04-01): */
/* Recommendation: ASM-only match. */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 -fno-expensive-optimizations */
/* @PostProc: strip_leading_instructions --count auto */
/* @Measured: 2026-09-24 10/103 exact (9.9%), compiled 101 vs target 103 words, Fixup 1 -- SEMI */
#include "types.h"

extern u32 *libcd_dma_exec_dicr_ptr;
extern u32 *libcd_dma_exec_dpcr_ptr;
extern void printf(char *fmt, u32 val);

void SD_BranchCTRL(s32 channel, u32 addr, u32 size1, u32 size2, u32 arg6, s32 arg4, u32 arg5) {
    volatile u32 syncVal;
    volatile u32 *hwReg;
    volatile u32 *dmaBase;
    u32 regVal;
    s32 timeout;
    u32 *ctrlPtr;
    u32 ctrlVal;
    u32 channelBit;

    hwReg = (volatile u32 *)(0x1F801088 + (channel << 4));

    regVal = *hwReg;
    if (regVal & 0x01000000) {
        timeout = 0;
        do {
            if (timeout == 0x10000) {
                printf((char *)0x800C8C70, *hwReg);
                break;
            }
            regVal = *hwReg;
            timeout++;
        } while (regVal & 0x01000000);
    }

    if (arg4 == 1) {
        ctrlPtr = libcd_dma_exec_dicr_ptr;
        channelBit = 0x10000 << channel;
        arg5 = arg5 << channel;
        channelBit = channelBit | arg5;
        ctrlVal = *ctrlPtr;
        ctrlVal = (ctrlVal & 0x00FFFFFF) | channelBit;
    } else {
        channelBit = 1 << (channel + 16);
        ctrlPtr = libcd_dma_exec_dicr_ptr;
        ctrlVal = *ctrlPtr;
        ctrlVal = ctrlVal & 0x00FFFFFF;
        ctrlVal = ctrlVal & ~channelBit;
    }
    *ctrlPtr = ctrlVal;

    syncVal = *(libcd_dma_exec_dicr_ptr);

    ctrlPtr = libcd_dma_exec_dpcr_ptr;
    ctrlVal = *ctrlPtr;
    ctrlVal |= (1 << ((channel << 2) + 3));
    *ctrlPtr = ctrlVal;

    dmaBase = (volatile u32 *)(0x1F801080 + (channel << 4));
    dmaBase[0] = addr;
    dmaBase[1] = (size1 << 16) | size2;
    dmaBase[2] = arg6;

    syncVal = dmaBase[2];
}
