/* Address: 0x8006C3D4 */
/* Size: 972 bytes (243 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -cckr -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u16 pass_days;

biorhythm_sf2(u, v, w, k)
s32 u;
s32 v;
s32 w;
s32 k;
{
    s32 unused0;
    s32 unused1;
    s32 base;
    s32 a;
    s32 b;
    s32 c;
    s32 f;
    s32 n;
    s32 sum;

    base = get_pass_days() + pass_days; a = rsin(base * 178);
    b = rsin(base * 146);
    c = rsin(base * 124);
    f = 0;
    n = 0;
    sum = 0;
    if (u) {
        n = 1;
        if (abs(a) < 409) {
            sum = -4096;
            f = 1;
        } else if (a > 0) {
            sum = a;
        } else {
            sum = -(a / 2);
        }
    }
    if (v) {
        n++;
        if (abs(b) < 409) {
            sum -= 4096;
            f++;
        } else if (b > 0) {
            sum += b;
        } else {
            sum -= b / 2;
        }
    }
    if (w) {
        n++;
        if (abs(c) < 409) {
            sum -= 4096;
            f++;
        } else if (c > 0) {
            sum += c;
        } else {
            sum -= c / 2;
        }
    }
    if (n == 0) {
        return 1;
    }
    if (k > 0) {
        if (sum / n < 0) {
            return 0;
        }
        return sum * k / 4096 / n;
    } else if (k < 0) {
        if (u) {
            return a * k / 4096;
        }
        if (v) {
            return b * k / 4096;
        }
        if (w) {
            return c * k / 4096;
        }
    } else {
        if (n == 0) {
            return 1;
        }
        if (n == 1) {
            if (f) {
                return -1;
            }
            return 1;
        }
        if (n == 2) {
            if (f) {
                return -1;
            }
            return 1;
        }
        if (n == 3) {
            if (f >= 2) {
                return -1;
            }
            return 1;
        }
        return 1;
    }
}
