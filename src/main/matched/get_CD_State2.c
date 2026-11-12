/* Address: 0x80046358 */
/* Size: 16 bytes (4 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 cd_state2;

u8 get_CD_State2(void) {
    return cd_state2;
}
