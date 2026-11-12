/* Address: 0x8007B6D0 */
/* Size: 12 bytes (3 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u16 gSDEvt;

void cmd_que_clear(void) {
    gSDEvt = 0;
}
