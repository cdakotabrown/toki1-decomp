/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=bb:22 IDO52_FORCE_WEB_COLOR=177:s3,178:s2  IDO52_AS1_TIE_REJECT=341 */
/* @CompileFlags: -cckr */
/* @SymbolSize: 900 */
/* option_sprite_move - OPTION overlay, Address: 0x80139514, Size: 900 bytes (225 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32  abs();
extern void _debug_number_show();
extern s32  rsin();
extern s32  rcos();

extern u8   class_w[];
extern s16  cursor_cw_x;
extern s16  cursor_cw_y;
extern s32  pad_trig;
extern s16  cal_cw_x;
extern s16  cal_cw_y;

option_sprite_move()
{
    u8 *p;
    s32 i;
    s32 n;
    s32 d;
    s32 t;
    s32 a;
    s32 dx;
    s32 dy;
    s32 x;

    n = 0;
    p = class_w;
    for (i = -100; i < 4; i += 8) {
        d = abs((cursor_cw_x - *(s16 *)(p + 6566)) & 0x3FF);
        _debug_number_show(
            abs((cursor_cw_y - *(s16 *)(p + 6570)) & 0x1FF) + d,
            -140, i, 6);

        d = abs((cursor_cw_x - *(s16 *)(p + 6566)) & 0x3FF);
        t = abs((cursor_cw_y - *(s16 *)(p + 6570)) & 0x1FF) + d;
        if (t < 32) {
            if ((pad_trig & 0x20) != 0) {
                a = *(u8 *)(p + 6531);
                if ((a & 0x10) != 0) {
                    *(u8 *)(p + 6531) = a & 0xFFEF;
                    if (*(s16 *)(p + 6566) < -512) {
                        *(s16 *)(p + 6566) = 512;
                    }
                    if (*(s16 *)(p + 6566) >= 513) {
                        *(s16 *)(p + 6566) = -512;
                    }
                    if (*(s16 *)(p + 6570) < -512) {
                        *(s16 *)(p + 6570) = 512;
                    }
                    if (*(s16 *)(p + 6570) >= 513) {
                        *(s16 *)(p + 6570) = -512;
                    }
                } else {
                    *(u8 *)(p + 6531) = a | 0x10;
                }
            }
        }

        if ((*(u8 *)(p + 6531) & 0x10) == 0) {
            t = *(s16 *)(p + 6536);
            *(s16 *)(p + 6536) = t + 1;
            *(s32 *)(p + 6568) += rsin(t << 6) * 4;
            n++;
        }
        p += 68;
    }

    _debug_number_show(n, 0, -100, 6);

    if (n == 13) {
        cal_cw_x = -150;
        cal_cw_y = -100;
        p = (&((CLASS_W *)class_w)[1].active);
        for (x = 0; x < 12; x++) {
            *(s16 *)(p + 6566) = -118 + (x * 32);
            *(s16 *)(p + 6570) = -100;
            p += 68;
        }
        return;
    }

    if (n >= 5) {
        for (x = 0; x < 13; x++) {
            p = &class_w[x * 68];
            if ((*(u8 *)(p + 6531) & 0x10) == 0) {
                dx = *(s16 *)(p + 6566)
                     - (rsin(*(s16 *)(p + 6536)) / 32);
                dy = *(s16 *)(p + 6570)
                     - (rcos(*(s16 *)(p + 6536)) / 40);
                if (dx < -16 || dx >= 17) {
                    if (dx > 0) {
                        *(s16 *)(p + 6566) = *(s16 *)(p + 6566) - 1;
                    } else {
                        *(s16 *)(p + 6566) = *(s16 *)(p + 6566) + 1;
                    }
                }
                if (dy < -16 || dy >= 17) {
                    if (dy > 0) {
                        *(s16 *)(p + 6570) = *(s16 *)(p + 6570) - 1;
                    } else {
                        *(s16 *)(p + 6570) = *(s16 *)(p + 6570) + 1;
                    }
                }
            }
        }
    }
}
