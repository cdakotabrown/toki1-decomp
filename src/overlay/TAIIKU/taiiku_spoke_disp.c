/* @CompileFlags: -cckr */
#include "types.h"

extern s32 work_miura;
extern s32 taiiku_ofs_y;
extern s32 taiiku_rot;
extern s16 taiiku_tama_sx;
void line_set();
void xy_rotate();

#define ANGLE(d) (taiiku_rot + (d) + m * 16)

void taiiku_spoke_disp()
{
    s32 i;
    s32 m;
    s32 a;
    s32 x[4];
    s32 y[4];
    s32 c;
    s32 r;
    s32 z;
    s32 k;
    s32 cx;
    s32 cy;
    s32 v;

    for (m = 0; m < 128; m++) {
        r = -320;
        z = 0;
        xy_rotate(ANGLE(0), &r, &z, &x[0], &y[0]);
        xy_rotate(ANGLE(16), &r, &z, &x[1], &y[1]);
        r = -240;
        z = 0;
        xy_rotate(ANGLE(0), &r, &z, &x[2], &y[2]);
        xy_rotate(ANGLE(16), &r, &z, &x[3], &y[3]);
        for (i = 0; i < 4; i++) {
            x[i] += work_miura;
            y[i] += taiiku_ofs_y;
        }
        for (i = 0; i < 4; i++) {
            y[i] = rcos(682) * y[i] >> 12;
        }
        cx = (x[0] + x[2]) >> 1;
        cy = (x[1] + x[3]) >> 1;
        for (i = 0; i < 2; i++) {
            k = i;
            if (y[k << 1] > 0 || y[(k << 1) + 1] > 0) {
                if (cx < x[k << 1]) {
                    x[k << 1] += y[k << 1] + cx;
                }
                if (x[k << 1] < cx) {
                    x[k << 1] = x[k << 1] - y[k << 1] + cx;
                }
                if (cy < x[(k << 1) + 1]) {
                    x[(k << 1) + 1] += y[(k << 1) + 1] + cy;
                }
                if (x[(k << 1) + 1] < cy) {
                    x[(k << 1) + 1] = x[(k << 1) + 1] - y[(k << 1) + 1] + cy;
                }
            }
        }
        for (i = 0; i < 4; i++) {
            x[i] = -x[i];
        }
        for (i = 0; i < 4; i++) {
            y[i] -= 24;
        }
        v = taiiku_tama_sx >> 2;
        for (i = 0; i < 4; i++) {
            x[i] -= v;
        }
        for (i = 0; i < 2; i++) {
            k = i;
            if ((x[k << 1] >= -160 && x[k << 1] <= 160 &&
                 y[k << 1] >= -23 && y[k << 1] <= 120) ||
                (x[(k << 1) + 1] >= -160 && x[(k << 1) + 1] <= 160 &&
                 y[(k << 1) + 1] >= -23 && y[(k << 1) + 1] <= 120)) {
                line_set((s16)x[k << 1], (s16)y[k << 1],
                              (s16)x[(k << 1) + 1], (s16)y[(k << 1) + 1],
                              0xFFFFFF, 0xFFFFFF, 250);
            }
        }
    }
}
