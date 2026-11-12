/* Function: MDEC_in_sync (== func_80086D60, symbol starts at the hoisted lui) */
/* Address: 0x80086D5C */
/* Size: 148 bytes (37 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc272 */
/* @CompileFlags: -O2 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x0 --offset2 0x4 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x14 */
#include "types.h"

extern u32 * volatile libpress_dma0_chcr_reg_ptr;
extern void timeout(const char *msg);

extern char libpress_mdec_in_sync_name_str[];

s32 func_80086D60(void) {
    volatile s32 counter;
    u32 *stat;

    stat = libpress_dma0_chcr_reg_ptr;
    counter = 0x100000;

    if ((*stat & 0x01000000) != 0) {
        do {
            counter = counter - 1;
            if (counter == -1) {
                timeout(libpress_mdec_in_sync_name_str);
                return -1;
            }
        } while (*libpress_dma0_chcr_reg_ptr & 0x01000000);
    }

    return 0;
}
