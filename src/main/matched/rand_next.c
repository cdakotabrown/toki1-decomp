/* Address: 0x800425D0 */
/* Size: 24 bytes (6 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s32 album_work;

s32 rand_next(void) {
    album_work = album_work + 887;
    return album_work;
}
