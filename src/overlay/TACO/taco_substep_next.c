/* Address: 0x8015A938 */
/* Size: 32 bytes (8 insns) */
#include "types.h"

extern s32 taco_phase_frame;
extern s32 taco_substep_no;

void taco_substep_next(void)
{
    taco_phase_frame = 0;
    taco_substep_no++;
}
