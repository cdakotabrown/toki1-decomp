/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=7:a3,22:a0,26:a2 */
#include "types.h"

typedef union {
    s32 l;
    struct {
        u16 lo;
        s16 hi;
    } w;
} Fix;

typedef struct {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
    u8 pad4[32];
    Fix x;
    Fix y;
    u8 pad2C[24];
} Actor;

typedef struct {
    u8 pad0[6];
    s16 v;
    s16 pad8;
    s16 w;
    u8 padC[4];
    u32 f0 : 1;
    u32 f1 : 1;
    u32 fpad : 30;
    u8 pad14[36];
} GirlRec;

typedef struct {
    u8 pad0[440];
    GirlRec rec[20];
} SaveData;

extern SaveData screen_w;
extern Actor class_w[];
extern s32 pad_data;

void taiiku_plate_scroll()
{
    s32 n;
    s32 i;
    s32 j;
    s32 k;
    s32 buf[11];

    n = 0;
    for (i = 0; i < 11; i++) {
        if (screen_w.rec[i].f1 == 1) {
            buf[n++] = i;
        }
    }
    k = 0;
    for (i = 0; i < 11; i++) {
        if (screen_w.rec[i].f1 == 1) {
            if (pad_data & 4) {
                class_w[96 + i].x.w.hi -= 4;
            } else {
                class_w[96 + i].x.w.hi -= 1;
            }
            if (class_w[96 + i].x.w.hi < -192) {
                class_w[96 + i].x.w.hi =
                    class_w[96 + buf[(k + n - 1) % n]].x.w.hi + 120;
                if (class_w[96 + i].x.w.hi < 160) {
                    class_w[96 + i].x.w.hi = 160;
                }
            }
            k++;
        }
    }
}
