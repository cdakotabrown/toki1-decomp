/* @SymbolStart: 0x80088370 */
/* @SymbolSize: 2260 */
/* Function: StCdInterrupt */
/* Address: 0x80088378 */
/* Size: 2244 bytes (561 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 --fill-load-delay --fill-branch-store --label-load-nop --sink-sp-adjust */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions 0x20 0x24 */
/* @PostProc: swap_instructions 0x24 0x28 */
/* @PostProc: swap_instructions 0xf0 0xf4 */
/* @PostProc: swap_instructions 0xf4 0xfc */
/* @PostProc: swap_instructions 0x114 0x118 */
/* @PostProc: swap_instructions 0x188 0x18c */
/* @PostProc: swap_instructions 0x1b0 0x1b8 */
/* @PostProc: swap_instructions 0x1b4 0x1bc */
/* @PostProc: swap_instructions 0x1b8 0x1c0 */
/* @PostProc: swap_instructions 0x1bc 0x1c4 */
/* @PostProc: swap_instructions 0x1dc 0x1e4 */
/* @PostProc: swap_instructions 0x1e0 0x1e8 */
/* @PostProc: swap_instructions 0x1e4 0x1f0 */
/* @PostProc: swap_instructions 0x1e8 0x1f0 */
/* @PostProc: swap_instructions 0x1ec 0x1f0 */
/* @PostProc: swap_instructions 0x238 0x23c */
/* @PostProc: swap_instructions 0x23c 0x240 */
/* @PostProc: swap_instructions 0x350 0x354 */
/* @PostProc: swap_instructions 0x354 0x358 */
/* @PostProc: swap_instructions 0x360 0x364 */
/* @PostProc: swap_instructions 0x3b0 0x3c0 */
/* @PostProc: swap_instructions 0x3b4 0x3c4 */
/* @PostProc: swap_instructions 0x3b8 0x3c0 */
/* @PostProc: swap_instructions 0x3bc 0x3c0 */
/* @PostProc: swap_instructions 0x3c0 0x3c8 */
/* @PostProc: swap_instructions 0x3c4 0x3c8 */
/* @PostProc: swap_instructions 0x3e4 0x3e8 */
/* @PostProc: swap_instructions 0x3f0 0x3f4 */
/* @PostProc: swap_instructions 0x44c 0x454 */
/* @PostProc: swap_instructions 0x450 0x458 */
/* @PostProc: swap_instructions 0x458 0x460 */
/* @PostProc: swap_instructions 0x478 0x488 */
/* @PostProc: swap_instructions 0x47c 0x48c */
/* @PostProc: swap_instructions 0x480 0x488 */
/* @PostProc: swap_instructions 0x484 0x488 */
/* @PostProc: swap_instructions 0x488 0x490 */
/* @PostProc: swap_instructions 0x48c 0x490 */
/* @PostProc: swap_instructions 0x518 0x51c */
/* @PostProc: swap_instructions 0x51c 0x520 */
/* @PostProc: swap_instructions 0x628 0x630 */
/* @PostProc: swap_instructions 0x62c 0x630 */
/* @PostProc: swap_instructions 0x630 0x63c */
/* @PostProc: swap_instructions 0x634 0x63c */
/* @PostProc: swap_instructions 0x638 0x63c */
/* @PostProc: swap_instructions 0x63c 0x640 */
/* @PostProc: swap_instructions 0x648 0x64c */
/* @PostProc: swap_instructions 0x64c 0x650 */
/* @PostProc: swap_instructions 0x650 0x654 */
/* @PostProc: swap_instructions 0x694 0x6a8 */
/* @PostProc: swap_instructions 0x698 0x6a4 */
/* @PostProc: swap_instructions 0x69c 0x6ac */
/* @PostProc: swap_instructions 0x6a0 0x6a8 */
/* @PostProc: swap_instructions 0x6a8 0x6b0 */
/* @PostProc: swap_instructions 0x6ac 0x6b4 */
/* @PostProc: swap_instructions 0x6b0 0x6b4 */
/* @PostProc: swap_instructions 0x6d0 0x6d4 */
/* @PostProc: swap_instructions 0x6d4 0x6d8 */
/* @PostProc: swap_instructions 0x6e8 0x6ec */
/* @PostProc: swap_instructions 0x6ec 0x6f0 */
/* @PostProc: swap_instructions 0x710 0x71c */
/* @PostProc: swap_instructions 0x714 0x720 */
/* @PostProc: swap_instructions 0x718 0x71c */
/* @PostProc: swap_instructions 0x71c 0x720 */
/* @PostProc: swap_instructions 0x760 0x764 */
/* @PostProc: swap_instructions 0x764 0x768 */
/* @PostProc: swap_instructions 0x768 0x76c */
/* @PostProc: swap_instructions 0x76c 0x774 */
/* @PostProc: swap_instructions 0x774 0x778 */
/* @PostProc: swap_instructions 0x79c 0x7a8 */
/* @PostProc: swap_instructions 0x7a0 0x7b0 */
/* @PostProc: swap_instructions 0x7a4 0x7ac */
/* @PostProc: swap_instructions 0x7a8 0x7b8 */
/* @PostProc: swap_instructions 0x7ac 0x7b8 */
/* @PostProc: swap_instructions 0x7b4 0x7b8 */
/* @PostProc: swap_instructions 0x7c4 0x7d0 */
/* @PostProc: swap_instructions 0x7c8 0x7d4 */
/* @PostProc: swap_instructions 0x7cc 0x7d8 */
/* @PostProc: swap_instructions 0x7d0 0x7dc */
/* @PostProc: swap_instructions 0x7d4 0x7e0 */
/* @PostProc: swap_instructions 0x7d8 0x7dc */
/* @PostProc: swap_instructions 0x808 0x80c */
/* @PostProc: swap_instructions 0x80c 0x810 */
/* @PostProc: swap_instructions 0x810 0x814 */
/* @PostProc: swap_instructions 0x814 0x81c */
/* @PostProc: swap_instructions 0x81c 0x820 */
/* @PostProc: swap_instructions 0x844 0x84c */
/* @PostProc: swap_instructions 0x848 0x850 */
/* @PostProc: swap_instructions 0x84c 0x858 */
/* @PostProc: swap_instructions 0x850 0x858 */
/* @PostProc: swap_instructions 0x854 0x858 */
/* @PostProc: swap_instructions 0x898 0x89c */
#include "types.h"

typedef void (*VoidFuncPtr)(void);

extern s32 CdReady(s32, u8 *);
extern void mem2mem(u32 *, u32 *, s32, s32);
extern void dma_execute(s32, u32 *, s32, s32, s32, s32, s32);
extern void init_ring_status(s32, s32);
extern void data_ready_callback(void);

extern s32 StRingAddr;
extern s32 libcd_st_dma_dst_ptr;
extern s32 StRgb24;
extern s32 StEmu_Addr;
extern s32 StEmu_Idx;
extern s32 StRingSize;
extern s32 StRingIdx1;
extern s32 StRingIdx2;
extern s32 StSTART_FLAG;
extern s32 StStartFrame;
extern s32 StEndFrame;
extern s32 StFinalSector;
extern s32 StFunc1;
extern VoidFuncPtr StFunc2;
extern s32 Stframe_no;
extern u16 Stsector_offset;
extern s32 CChannel;
extern s32 StCHANNEL;
extern s32 StCdIntrFlag;

extern volatile u32 *libcd_stintr_dma1_chcr_ptr;
extern volatile u8 *libcd_stintr_index_reg_ptr;
extern volatile u8 *libcd_stintr_response_reg_ptr;
extern u8 *libcd_stintr_irq_reg_ptr;
extern volatile u32 *libcd_stintr_cdrom_delay_ptr;
extern volatile u32 *libcd_stintr_com_delay_ptr;
extern volatile u32 *libcd_stintr_dma3_chcr_ptr;
extern s32 debug_cause;
extern u16 libcd_stintr_state_hi;

extern volatile u16 *libcd_st_ring_slot_ptr;

void StCdInterrupt(void)
{
    volatile u16 sp20;
    volatile u16 sp22;
    volatile u16 sp24;
    u8 buffer[8];
    s32 dmaMode;

    if (StFinalSector == 1) {
        return;
    }

    sp24 = 0x5555;
    sp22 = sp24;

    if (StRgb24 != 0) {
        if ((*libcd_stintr_dma1_chcr_ptr & 0x01000000) != 0) {
            StCdIntrFlag = 1;
            if (StEmu_Addr != 0) {
                StEmu_Idx = StEmu_Idx + 1;
            }
            debug_cause = 1;
            return;
        }
    }

    if (CdReady(1, buffer) == 5) {
        return;
    }

    sp22 = buffer[0];
    sp24 = buffer[1];
    sp20 = libcd_stintr_index_reg_ptr[0];

    if (sp22 & 0x4) {
        sp24 = libcd_stintr_response_reg_ptr[0];
        debug_cause = 3;
        return;
    }

    libcd_st_ring_slot_ptr = (volatile u16 *)(StRingAddr + (StRingIdx1 << 5));

    if (libcd_st_ring_slot_ptr[0] != 0) {
        if (StEmu_Addr != 0) {
            StEmu_Idx = StEmu_Idx + 1;
        }
        debug_cause = 4;
        return;
    }

    libcd_stintr_index_reg_ptr[0] = 0;
    *libcd_stintr_irq_reg_ptr = 128;
    *libcd_stintr_cdrom_delay_ptr = 0x00020943;
    *libcd_stintr_com_delay_ptr = 4899;

    dmaMode = 0x11000000;
    if (StEmu_Addr != 0) {
        mem2mem((u32 *)libcd_st_ring_slot_ptr,
                       (u32 *)((StEmu_Idx << 11) + StEmu_Addr), 8, 0);
    } else {
        dma_execute(3, (u32 *)libcd_st_ring_slot_ptr, 0, 8, dmaMode, 0, 0);
    }

    while ((*libcd_stintr_dma3_chcr_ptr & 0x01000000) != 0) {
    }

    *libcd_stintr_cdrom_delay_ptr = 0x00020843;
    *libcd_stintr_com_delay_ptr = 4901;

    if (StSTART_FLAG == 1 && StStartFrame != 0) {
        if (StStartFrame == libcd_st_ring_slot_ptr[4]) {
            StSTART_FLAG = 0;
        } else {
            libcd_st_ring_slot_ptr[0] = 0;
            if (StEmu_Addr != 0) {
                StEmu_Idx = StEmu_Idx + 1;
            }
            libcd_stintr_index_reg_ptr[0] = 0;
            *libcd_stintr_irq_reg_ptr = 0;
            return;
        }
    }

    if (libcd_st_ring_slot_ptr[0] == 352) {
        if (((libcd_st_ring_slot_ptr[1] >> 10) & 0x1f) == StCHANNEL) {
            goto L_6e4;
        }
    }

    if (StEmu_Addr != 0) {
        StEmu_Idx = 0;
    } else {
        libcd_st_ring_slot_ptr[0];
    }

    debug_cause = 5;
    libcd_st_ring_slot_ptr[0] = 0;
    libcd_stintr_index_reg_ptr[0] = 0;
    *libcd_stintr_irq_reg_ptr = 0;
    return;

L_6e4:
    if ((s16)Stsector_offset != libcd_st_ring_slot_ptr[2]) {
        goto L_720;
    }

    if (Stframe_no == 0) {
        goto L_79c;
    }
    if ((u32)Stframe_no == (u32)libcd_st_ring_slot_ptr[4]) {
        goto L_79c;
    }

L_720:
    Stframe_no = 0;
    Stsector_offset = 0;
    init_ring_status(StRingIdx2, StRingIdx1 - StRingIdx2);
    StRingIdx1 = StRingIdx2;
    libcd_st_ring_slot_ptr[0] = 0;
    if (StEmu_Addr != 0) {
        StEmu_Idx = StEmu_Idx + 1;
    }
    debug_cause = 6;
    libcd_stintr_index_reg_ptr[0] = 0;
    *libcd_stintr_irq_reg_ptr = 0;
    return;

L_79c:
    if (libcd_st_ring_slot_ptr[2] != 0) {
        goto L_9f4_with_10;
    }

    Stsector_offset = 0;
    Stframe_no = libcd_st_ring_slot_ptr[4] & 0xFFFF;

    if (StEndFrame == 0 || (u32)Stframe_no < (u32)StEndFrame) {
        goto L_884;
    }

    Stframe_no = 0;
    Stsector_offset = 0;
    init_ring_status(StRingIdx2, StRingIdx1 - StRingIdx2);
    StRingIdx1 = StRingIdx2;
    libcd_st_ring_slot_ptr[0] = 0;

    StSTART_FLAG = 1;
    if (StFunc2 != 0) {
        StFunc2();
    }

    if (StEmu_Addr != 0) {
        StEmu_Idx = StEmu_Idx + 1;
    }
    debug_cause = 7;
    libcd_stintr_index_reg_ptr[0] = 0;
    *libcd_stintr_irq_reg_ptr = 0;
    return;

L_884:
    {
        s32 relOffset;
        u16 trackCount;

        relOffset = StRingSize - StRingIdx1;
        trackCount = libcd_st_ring_slot_ptr[3];
        relOffset = relOffset - 1;

        if ((u32)relOffset >= (u32)trackCount) {
            goto L_9e0;
        }
    }

    if (StEndFrame != 0) {
        goto L_924;
    }

    libcd_st_ring_slot_ptr[0] = 1;
    StSTART_FLAG = 1;
    if (StFunc2 != 0) {
        StFunc2();
    }

    if (StEmu_Addr != 0) {
        StEmu_Idx = StEmu_Idx + 1;
    }
    debug_cause = 8;
    libcd_stintr_index_reg_ptr[0] = 0;
    *libcd_stintr_irq_reg_ptr = 0;
    return;

L_924:
    {
        s16 *basePtr = (s16 *)StRingAddr;
        if (*basePtr == 0) {
            goto L_994;
        }
    }

    libcd_st_ring_slot_ptr[0] = 0;
    if (StEmu_Addr != 0) {
        StEmu_Idx = StEmu_Idx + 1;
    }
    debug_cause = 9;
    libcd_stintr_index_reg_ptr[0] = 0;
    *libcd_stintr_irq_reg_ptr = 0;
    return;

L_994:
    libcd_st_ring_slot_ptr[0] = 1;
    {
        u32 *src;
        u32 *dst;
        u32 i;

        StRingIdx1 = 0;
        dst = (u32 *)StRingAddr;
        src = (u32 *)libcd_st_ring_slot_ptr;
        for (i = 0; i < 8; i++) {
            *dst++ = *src++;
        }
        libcd_st_ring_slot_ptr = (volatile u16 *)StRingAddr;
    }

L_9e0:
    StRingIdx2 = StRingIdx1;

L_9f4_with_10:
    debug_cause = 10;
    Stsector_offset = Stsector_offset + 1;

    {
        s32 v1val = StRingSize;
        s32 base = StRingAddr;
        s32 curIdx = StRingIdx1;
        libcd_st_dma_dst_ptr = (v1val << 5) + base + ((curIdx << 6) - curIdx) * 32;
    }

    if (StRgb24 != 0) {
        dmaMode = 0x11000000;
        *libcd_stintr_cdrom_delay_ptr = 0x00020943;
        *libcd_stintr_com_delay_ptr = 4899;
    } else {
        *libcd_stintr_cdrom_delay_ptr = 0x21020843;
        dmaMode = 0x11400100;
    }
    if ((libcd_st_ring_slot_ptr[3] - 1) == libcd_st_ring_slot_ptr[2]) {
        if (StEmu_Addr != 0) {
            mem2mem((u32 *)libcd_st_dma_dst_ptr,
                           (u32 *)((StEmu_Idx << 11) + StEmu_Addr + 32), 504, 1);
            StEmu_Idx = StEmu_Idx + 1;
        } else {
            dma_execute(3, (u32 *)libcd_st_dma_dst_ptr, 0, 504, dmaMode, 1, 0);
        }

        Stsector_offset = 0;
        Stframe_no = 0;
        StFinalSector = 1;
        StCHANNEL = CChannel;
    } else {
        if (StEmu_Addr != 0) {
            mem2mem((u32 *)libcd_st_dma_dst_ptr,
                           (u32 *)((StEmu_Idx << 11) + StEmu_Addr + 32), 504, 0);
            StEmu_Idx = StEmu_Idx + 1;
        } else {
            dma_execute(3, (u32 *)libcd_st_dma_dst_ptr, 0, 504, dmaMode, 0, 0);
        }
    }

    *libcd_stintr_com_delay_ptr = 4901;
    libcd_st_ring_slot_ptr[0] = 3;

    StRingIdx1 = StRingIdx1 + 1;
    if (StEmu_Addr != 0) {
        if (StFinalSector != 0) {
            data_ready_callback();
        }
    }
}
