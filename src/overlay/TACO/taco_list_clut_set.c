/* @CompileEnv: IDO52_AS1_NO_PASS1_PROP=13,14,15 IDO52_AS1_TIE_TAKE=29,38,47,56,65,74,82 */
/* Address: 0x801546FC, Size: 292 bytes (73 insns) */
#include "types.h"

extern s32 taco_addr_check();

void taco_list_clut_set(u32 *p, u16 a1, s32 a2, s32 a3)
{
    s32 s1, s2;
    u32 v0, w, t0;
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
        t0 = (a3 << 22) + (((a2 >> 4) & 0x3F) << 16);
        v0 = p[0];
        if (k1 == v0 || k2 == v0) {
            v0 = p[2];
            p[1] = (p[1] & 0xFFFF) + t0;
            p += 2;
            *p = (v0 & 0xFFFF) + (v0 & 0x01E00000) + ((a1 & 0x1F) << 16);
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
