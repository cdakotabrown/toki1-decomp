/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic0 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=320,240 IDO52_NO_LOOP_TEST_REPLACEMENT=1 IDO52_COPYPROP_VAR=-88 IDO52_FORCE_WEB_COLOR=95:s2,11:v1,15:a1,16:a0,79:a3,82:t0,235:a1,30:v1,260:v0,226:a3,227:t0 IDO52_FORCE_WEB_SPILL=237 IDO52_SPLIT_BLOCKS=30#1:52,30.1#1:100,30.2#1:49,226#1:1 IDO52_FORCE_WEB_NOSPILL=30.0:5,30.1/-1:5,30.2/-1:5 */
/* Address: 0x8013EE38 */
/* Size: 1788 bytes (447 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 work_miura;
extern s32 taiiku_ofs_y;
extern s32 taiiku_rot;
extern s16 taiiku_tama_sx;
extern CLASS_W class_w[];

extern void xy_rotate(s32, s32 *, s32 *, s32 *, s32 *);
extern s32 rcos(s32);
extern void line_set(s16, s16, s16, s16, s32, s32, s32);

void taiiku_sweep_disp(void)
{
    s32 tx;
    s32 ty;
    s32 k;
    s32 p[4];
    s32 d[4];
    s32 q;
    s32 va;
    s32 vb;
    s32 i;
    s32 j;
    s32 n;
    s32 gx;
    s32 gy;
    s32 x;
    s32 sc;
    s32 cx;
    s32 cy;
    s32 dx;

    for (i = 0; i < 64; i = x) {
        p[1] = 320;
        p[3] = 240;
        d[2] = i << 3;
        d[3] = (i + 1) << 3;
        d[1] = (i + 1) << 3;
        d[0] = i << 3;
        p[2] = 240;
        p[0] = 320;
        x = i + 1;

        for (n = 0; n <= 3; n++) {
            va = p[n];
            vb = d[n];
            xy_rotate(taiiku_rot, &va, &vb, &p[n], &d[n]);
        }

        gx = work_miura;
        gy = taiiku_ofs_y;
        for (n = 0; n <= 3; n++) {
            p[n] = p[n] + gx;
            d[n] = d[n] + gy;
        }

        for (n = 0; n <= 3; n++) {
            sc = rcos(682);
            d[n] = (s32)((u32)d[n] * (u32)sc) >> 12;
        }

        q = p[0];
        tx = p[2];
        cx = (tx + q) >> 1;
        cy = (p[1] + p[3]) >> 1;

        for (j = 0; j < 2; j++) {
            dx = d[j << 1];
            if (dx > 0 || d[(j << 1) + 1] > 0) {
                if (cx < p[j << 1]) {
                    p[j << 1] = p[j << 1] + dx + cx;
                }
                if (p[j << 1] < cx) {
                    p[j << 1] = p[j << 1] - dx + cx;
                }
                if (cy < p[(j << 1) + 1]) {
                    p[(j << 1) + 1] = d[(j << 1) + 1] + p[(j << 1) + 1] + cy;
                }
                if (p[(j << 1) + 1] < cy) {
                    p[(j << 1) + 1] = p[(j << 1) + 1] - d[(j << 1) + 1] + cy;
                }
            }
        }

        for (n = 0; n <= 3; n++) {
            p[n] = -p[n];
        }

        for (n = 0; n <= 3; n++) {
            d[n] = d[n] - 24;
        }

        k = taiiku_tama_sx >> 2;
        for (n = 0; n <= 3; n++) {
            p[n] = p[n] - k;
        }

        for (j = 0; j != 2; j++) {
            if ((p[j << 1] >= -160 && p[j << 1] <= 160 &&
                 d[j << 1] >= -23 && d[j << 1] <= 120) ||
                (p[(j << 1) + 1] >= -160 && p[(j << 1) + 1] <= 160 &&
                 d[(j << 1) + 1] >= -23 && d[(j << 1) + 1] <= 120)) {
                line_set((s16)p[j << 1], (s16)d[j << 1],
                              (s16)p[(j << 1) + 1], (s16)d[(j << 1) + 1],
                              0x00FFFFFF, 0x00FFFFFF, 250);
            }
        }
    }

    class_w[102].x.half[1] = (s16)(p[3] + 8);

    for (x = 0; x != 1024; x += 16) {
        va = 0;
        vb = -320;
        xy_rotate(taiiku_rot + x, &va, &vb, p, d);
        xy_rotate(taiiku_rot + x + 16, &va, &vb,
                      p + 1, d + 1);
        va = 0;
        vb = -240;
        xy_rotate(taiiku_rot + x, &va, &vb, p + 2, d + 2);
        xy_rotate(taiiku_rot + x + 16, &va, &vb,
                      p + 3, d + 3);

        gx = work_miura;
        gy = taiiku_ofs_y;
        for (n = 0; n <= 3; n++) {
            p[n] = p[n] + gx;
            d[n] = d[n] + gy;
        }

        for (n = 0; n <= 3; n++) {
            sc = rcos(682);
            d[n] = (s32)((u32)d[n] * (u32)sc) >> 12;
        }

        q = p[0];
        tx = p[2];
        cx = (tx + q) >> 1;
        cy = (p[1] + p[3]) >> 1;

        for (j = 0; j < 2; j++) {
            dx = d[j << 1];
            if (dx > 0 || d[(j << 1) + 1] > 0) {
                if (cx < p[j << 1]) {
                    p[j << 1] = p[j << 1] + dx + cx;
                }
                if (p[j << 1] < cx) {
                    p[j << 1] = p[j << 1] - dx + cx;
                }
                if (cy < p[(j << 1) + 1]) {
                    p[(j << 1) + 1] = d[(j << 1) + 1] + p[(j << 1) + 1] + cy;
                }
                if (p[(j << 1) + 1] < cy) {
                    p[(j << 1) + 1] = p[(j << 1) + 1] - d[(j << 1) + 1] + cy;
                }
            }
        }

        for (n = 0; n <= 3; n++) {
            p[n] = -p[n];
        }

        for (n = 0; n <= 3; n++) {
            d[n] = d[n] - 24;
        }

        k = taiiku_tama_sx >> 2;
        for (n = 0; n <= 3; n++) {
            p[n] = p[n] - k;
        }

        for (j = 0; j != 2; j++) {
            if ((p[j << 1] >= -160 && p[j << 1] <= 160 &&
                 d[j << 1] >= -23 && d[j << 1] <= 120) ||
                (p[(j << 1) + 1] >= -160 && p[(j << 1) + 1] <= 160 &&
                 d[(j << 1) + 1] >= -23 && d[(j << 1) + 1] <= 120)) {
                line_set((s16)p[j << 1], (s16)d[j << 1],
                              (s16)p[(j << 1) + 1], (s16)d[(j << 1) + 1],
                              0x00FFFFFF, 0x00FFFFFF, 250);
            }
        }
    }
}
