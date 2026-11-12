/* Address: 0x8006CCC8  Size: 188 bytes (47 insns) */
/* Region: Game (0x80041000-0x80086000) */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v1:24,a0:27 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v1:24,a0:27 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 last_date_spot_timer;
extern u8 last_date_spot_timer1;
extern u8 last_date_spot_timer2;
extern u8 D_800EE763[];
extern u8 last_date_spot_timer_end[];

void last_date_spot_timer_dec(void) {
    u8 *v1;
    u8 *stop;

    if (last_date_spot_timer != 0) last_date_spot_timer = (u8)(last_date_spot_timer - 1);
    if (last_date_spot_timer1 != 0) last_date_spot_timer1 = (u8)(last_date_spot_timer1 - 1);
    if (last_date_spot_timer2 != 0) last_date_spot_timer2 = (u8)(last_date_spot_timer2 - 1);

    v1   = D_800EE763;
    stop = last_date_spot_timer_end;
    do {
        if (v1[1852] != 0) v1[1852] = (u8)(v1[1852] - 1);
        if (v1[1853] != 0) v1[1853] = (u8)(v1[1853] - 1);
        if (v1[1854] != 0) v1[1854] = (u8)(v1[1854] - 1);
        if (v1[1855] != 0) v1[1855] = (u8)(v1[1855] - 1);
        v1 += 4;
    } while (v1 != stop);
}
