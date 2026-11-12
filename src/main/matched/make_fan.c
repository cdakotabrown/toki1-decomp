/* Address: 0x80046A94 */
/* Size: 884 bytes (221 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

void load_image_rect();

void make_fan(buf, mode, level)
s32 *buf;
s32 mode;
s32 level;
{
    s32 x;
    s32 y;
    s32 k;
    s32 d;
    u8 v;
    s32 *p;

    for (y = 0, p = buf; y < 16; y++, p++) {
        for (x = 0; x < 128; x++) {
            p[x * 16] = 0;
        }
    }
    if ((u8)(level / 16) != 0) {
        for (y = 0; y < 128; y++) {
            for (x = 0; x < 128; x++) {
                d = y * y + x * x;
                for (k = 15; k != 0; k--) {
                    if (d < ((15 - k) * (15 - k) << 6)) {
                        buf[y / 8 + x * 16] |= k << ((y % 8) << 2);
                        break;
                    }
                }
            }
        }
    } else {
        v = level & 0xF;
        for (y = 0; y < 128; y++) {
            for (x = 0; x < 128; x++) {
                if (v != 0) {
                    if (y * y + x * x < 16129) {
                        buf[y / 8 + x * 16] |= v << ((y % 8) << 2);
                    }
                } else {
                    if (y * y + x * x > 16129) {
                        buf[y / 8 + x * 16] |= 1 << ((y % 8) << 2);
                    }
                }
            }
        }
    }
    load_image_rect((u16)((mode & 0xF) << 6), (u16)(((mode & 0x10) << 4) + ((mode & 0x20) << 2)), 32, 128, buf);
}
