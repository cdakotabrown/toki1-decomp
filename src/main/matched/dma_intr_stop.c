/* Function: dma_intr_stop */
/* Address: 0x800B448C */
/* Size: 68 bytes (17 instructions) */
/* Region: Library (0x80086000+) */
/* Likely Symbol: startIntrDMA (from o.bin) */
/* @Compiler: gcc260 */
/* @CompileFlags: --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: maspsx --aspsx-version=2.67 */
/* @PostProc: fill_gcc_delay_slots */
#include "types.h"

extern volatile s16 libapi_dma_intr_enabled;
extern s32 *libetc_dma_dicr_ptr;

void dma_intr_table_clear(volatile s16 *arg0, s32 arg1);
void InterruptCallback(s32 arg0, s32 arg1);

void dma_intr_stop(void) {
    volatile s16 *p = &libapi_dma_intr_enabled;
    *p = 0;
    dma_intr_table_clear(p, 9);
    *libetc_dma_dicr_ptr = 0;
    InterruptCallback(3, 0);
}
