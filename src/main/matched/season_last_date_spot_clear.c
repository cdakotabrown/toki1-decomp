/* Address: 0x8006D0D4 */
/* Size: 256 bytes (64 instructions) */
/* Region: Game */
/* @PostProc: strip_leading_instructions --count auto */
/* @CompileEnv: IDO52_NO_CSE_OPCODES=24 IDO52_FORCE_WEB_COLOR=44:a2 */
/* @CompileFlags: -Wc,-pic1 -Wo,-loopunroll,0 */
#include "types.h"

extern u8 last_date_spot;
extern u8 last_date_spot1;
extern u8 last_date_spot2;
extern u8 last_date_spot_flag;
extern u8 last_date_spot_flag1;
extern u8 last_date_spot_flag2;
extern u8 D_800EE763[];

void season_last_date_spot_clear(void) {
    u8 v;
    u8 *q;
    u8 *qend;

    v = last_date_spot;
    if (v == 35 || v == 36) last_date_spot = 0;
    last_date_spot_flag = 0;

    v = last_date_spot1;
    if (v == 35 || v == 36) last_date_spot1 = 0;
    last_date_spot_flag1 = 0;

    v = last_date_spot2;
    if (v == 35 || v == 36) last_date_spot2 = 0;

    q = D_800EE763; last_date_spot_flag2 = 0; qend = D_800EE763 + 8;
    do {
        v = q[1836]; if (v == 35 || v == 36) q[1836] = 0;
        q[1868] = 0;
        v = q[1837]; if (v == 35 || v == 36) q[1837] = 0;
        q[1869] = 0;
        v = q[1838]; if (v == 35 || v == 36) q[1838] = 0;
        q[1870] = 0;
        v = q[1839]; if (v == 35 || v == 36) q[1839] = 0;
        q += 4;
        q[1867] = 0;
    } while (q != qend);

}
