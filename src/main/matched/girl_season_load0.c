/* Likely Symbol: device_init_step (from o.bin, offset +228 bytes from 0x80084944) */
/* Address: 0x80084860 - 0x80084880 (32 bytes) */
/* Region: IDO (>= 0x80081000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void girl_season_load(void);

void girl_season_load0(void) {
    girl_season_load();
}
