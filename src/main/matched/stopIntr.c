/* Function: stopIntr */
/* Address: 0x800B3F90 */
/* Size: 104 bytes (26 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern void EnterCriticalSection(void);
extern void memclr(u32 *ptr, s32 count);
extern void vsync_intr_stop(void);
extern void dma_intr_stop(void);
extern void ResetEntryInt(void);
extern void ExitCriticalSection(void);

extern volatile u16 *libapi_trapintr_stat_reg_ptr;
extern u16 *_mask;

extern u32 libapi_intr_env[25];

void stopIntr(void)
{
    EnterCriticalSection();
    memclr(libapi_intr_env, 25);
    *_mask = *libapi_trapintr_stat_reg_ptr = 0;
    vsync_intr_stop();
    dma_intr_stop();
    ResetEntryInt();
    ExitCriticalSection();
}
