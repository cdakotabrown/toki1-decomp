/* Function: data_ready_callback */
/* Address: 0x8008B140 */
/* Size: 132 bytes (33 instructions) */
/* Region: Library (0x80086000+) */
/* Compiler: Psy-Q GCC 2.6.0 -O2 */
/* @SymbolStart: 0x8008B140 */
/* @SymbolSize: 132 */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef void (*FuncPtr)(void);

extern u8 *StRingAddr;
extern s32 StRingIdx1;
extern s32 StRingIdx2;
extern s32 StFinalSector;
extern FuncPtr StFunc1;
extern u8 *libcd_dataready_index_reg_ptr;
extern u8 *libcd_dataready_irq_reg_ptr;

void data_ready_callback(void)
{
    s32 next;
    FuncPtr callback;

    *(s16 *)(StRingAddr + StRingIdx2 * 32) = 2;
    next = StRingIdx1;
    callback = StFunc1;
    StFinalSector = 0;
    StRingIdx2 = next;
    if (callback != 0) {
        callback();
    }
    *libcd_dataready_index_reg_ptr = 0;
    *libcd_dataready_irq_reg_ptr = 0;
}
