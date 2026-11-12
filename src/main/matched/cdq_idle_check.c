/* Address: 0x800454B8 */
/* Size: 60 bytes (15 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 cdq_head;
extern u8 cdq_tail;
extern s8 cd_head_pos;

s32 cdq_idle_check(void) {
    if (cdq_tail == cdq_head) {
        if (cd_head_pos == -1) {
            return 1;
        }
    }
    return 0;
}
