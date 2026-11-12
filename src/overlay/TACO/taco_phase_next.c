/* Address: 0x8015A910 */
/* Size: 40 bytes (10 insns) */
#include "types.h"

extern s32 taco_phase_cnt;
extern s32 taco_phase_no;
extern s32 taco_emit_mode;

void taco_phase_next(void)
{
    taco_phase_cnt = 0;
    taco_phase_no++;
    taco_emit_mode = 0;
}
