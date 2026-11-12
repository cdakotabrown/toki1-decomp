/* @SymbolSize: 92 */
/* Address: 0x8013ECCC */
/* Size: 92 bytes (23 instructions) */
#include "types.h"

extern s32 etc_bustup_flag;
extern s32 etc_event_flag;

extern void etc_step_main();
extern void etc_bustup_init();
extern void etc_record_reset();

void etc_sub_init(void)
{
    etc_record_reset();
    if (etc_bustup_flag == 1) {
        etc_bustup_init();
    } else if (etc_event_flag == 1) {
        etc_step_main();
    }
}
