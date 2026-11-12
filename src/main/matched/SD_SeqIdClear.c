/* Address: 0x8007C038 */
/* Size: 16 bytes (4 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s16 seq_req;

void SD_SeqIdClear(void) {
    seq_req = -1;
}
