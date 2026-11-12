/* Address: 0x800425A4 */
/* Size: 44 bytes (11 instructions) */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s32 vblnk_counter;
extern s32 album_work;

void vblank_counter_tick(void) {
    vblnk_counter = vblnk_counter + 1;
    album_work = album_work + 887;
}
