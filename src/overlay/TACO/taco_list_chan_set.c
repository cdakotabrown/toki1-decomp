/* @CompileEnv: IDO52_COPYPROP_VAR=-16,-20,-24 IDO52_FORCE_WEB_COLOR=24:t0,14:s4,17:s5,26:s3 IDO52_UGEN_DEST_TEMP=8 IDO52_UGEN_TEMP_REG=1:t9 IDO52_AS1_FORCE_LA_PULL=0 IDO52_AS1_NO_PASS1_PROP=13,14,15 IDO52_AS1_TIE_TAKE=23,30,37,44,50 IDO52_AS1_TIE_REJECT=85 */
/* Address: 0x80154200, Size: 236 bytes (59 insns; .sym records 236) */
#include "types.h"

extern s32 taco_addr_check();

void taco_list_chan_set(u32 *p, u16 a1)
{
    s32 s1, s2;
    u32 v0, t0;
    u32 k1, k2;

    s1 = p[1];
    s2 = p[3];
    p += 9;
    k1 = 0x24000507;
    k2 = 0x34000609;
    for (;;) {
        if (taco_addr_check(p) == 0) {
            break;
        }
        t0 = (u32)(a1 & 0x1F) << 16;
        v0 = p[0];
        if (k1 == v0 || k2 == v0) {
            v0 = p[2];
            p += 2;
            *p = (v0 & 0xFFFF) + (v0 & 0x01E00000) + t0;
        }
        p += 1;
        if (s1 != 0 && (u32)s2 < (u32)p) {
            break;
        }
        if (s1 != 0) {
            continue;
        }
        if ((u32)(&p[s2] + 3) < (u32)p) {
            break;
        }
    }
}
