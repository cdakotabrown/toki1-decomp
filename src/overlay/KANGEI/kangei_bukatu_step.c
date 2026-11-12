/* @SymbolStart: 0x80133A24 */
/* @SymbolSize: 64 */
/* kangei_bukatu_step - KANGEI overlay, Address: 0x80133A40, Size: 44 bytes (11 insns) */
#include "types.h"

typedef struct {
    u32 frame;
    u8  pad4;
    u8  step1;
    u8  step;
    u8  pad7;
} KangeiStep;
extern KangeiStep step_counter;
extern void kangei_bukatu_para_set(void);

void
kangei_bukatu_step(void)
{
    step_counter.frame += 1;
    if (step_counter.step != 0) {
        return;
    }
    kangei_bukatu_para_set();
}
