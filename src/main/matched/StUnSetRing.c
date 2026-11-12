/* Function: StUnSetRing */
/* Address: 0x80088260 */
/* Size: 80 bytes (20 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 */
/* @PostProc: strip_leading_instructions --count 19 */
#include "types.h"

extern void EnterCriticalSection(void);
extern void CdDataCallback(s32 a0);
extern void CdReadyCallback(s32 a0);
extern void ExitCriticalSection(void);

extern u8 *libcd_unsetring_index_reg_ptr;
extern u8 *libcd_unsetring_irq_reg_ptr;

void StUnSetRing(void) {
    EnterCriticalSection();
    CdDataCallback(0);
    CdReadyCallback(0);
    *libcd_unsetring_index_reg_ptr = 0;
    *libcd_unsetring_irq_reg_ptr = 0;
    ExitCriticalSection();
}
