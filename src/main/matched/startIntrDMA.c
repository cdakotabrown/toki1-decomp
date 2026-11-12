/* Function: startIntrDMA */
/* Address: 0x800B4430 */
/* Size: 92 bytes (23 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field --patch 0x3c:rt:v0:v1 --patch 0x40:rt:v0:v1 */
#include "types.h"

extern s16 libapi_dma_intr_enabled;
extern s32 *libetc_dma_dicr_ptr;
extern void dma_intr_table_clear(s16 *arg0, s32 arg1);
extern void InterruptCallback(s32 arg0, void *arg1);
extern void trapIntrDMA();
extern void setIntrDMA();

s8 *startIntrDMA(void) {
    s16 *p = &libapi_dma_intr_enabled;

    dma_intr_table_clear(p, 9);
    *libetc_dma_dicr_ptr = 0;
    InterruptCallback(3, (s8 *)trapIntrDMA);
    *p = 1;
    return (s8 *)setIntrDMA;
}
