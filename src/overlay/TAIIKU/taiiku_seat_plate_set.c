/* @SymbolSize: 752 */
/* @CompileFlags: -cckr -Wc,-pic0 */
/* @PostProc: remap_stack_slots --map 40:56 --keep-frame */
/* @PostProc: swap_instructions --offset1 0x158 --offset2 0x15C */
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
    u8 b0;
    u8 b1;
    u8 b2;
    u8 pad3[33];
} Sel;

extern u8 taiiku_kari_hop[];
extern u8 taiiku_kari_hop_tmr[];
extern Actor class_w[];
extern Sel taiiku_kari_run[];
extern s16 taiiku_kari_scrl_hi;
extern u8 taiiku_kari_gseat;
extern s32 taiiku_konami_mode;

void taiiku_seat_plate_set()
{
    s32 i;
    s32 k;
    u32 v;
    s32 y;
    s32 x;
    s32 n;
    s32 lim;

    for (i = 0; i < 2; i++) {
        v = ++taiiku_kari_hop_tmr[i];
        if (v >= 121) {
            taiiku_kari_hop_tmr[i] = 0;
        } else if (v == 10) {
            taiiku_kari_hop[i] = rand() % 20 + 10;
        } else if (v == 27) {
            taiiku_kari_hop[i] = 255;
        } else if (v >= 19 && taiiku_kari_hop[i] != 255) {
            class_w[taiiku_kari_hop[i] + 46].y.w.hi++;
        } else if (v >= 11 && taiiku_kari_hop[i] != 255) {
            class_w[taiiku_kari_hop[i] + 46].y.w.hi--;
        }
    }

    lim = taiiku_kari_gseat;
    for (i = 0, x = 0, y = 480 - taiiku_kari_scrl_hi; i < 30; i++) {
        if (x >= 320) {
            x -= 320;
            y -= 320;
        }
        class_w[i + 46].x.w.hi = y;
        if (i >= 10 && i < 20) {
            class_w[i + 46].x.w.hi += 16;
        }
        if (i == 19) {
            class_w[i + 46].b2 = 0;
            class_w[i + 46].b3 = 0;
        } else if (class_w[i + 46].x.w.hi >= -207 &&
                   class_w[i + 46].x.w.hi < 321) {
            class_w[i + 46].b2 = 1;
            class_w[i + 46].b3 = 164;
        } else {
            class_w[i + 46].b2 = 0;
            class_w[i + 46].b3 = 0;
        }
        for (k = 0; k < 4; k++) {
            if (i == lim && class_w[107].b2 != 0 && (taiiku_konami_mode | 0) == 1) {
                n = taiiku_kari_run[k].b2;
                class_w[i + 46].b2 = 0;
                class_w[i + 46].b3 = 0;
            } else {
                n = taiiku_kari_run[k].b2;
                if (i == n) {
                    if (class_w[k + 107].b2 != 0) {
                        if (k == 0) {
                            if ((taiiku_konami_mode | 0) == 255) {
                                class_w[i + 46].b2 = 0;
                                class_w[i + 46].b3 = 0;
                            }
                        } else {
                            class_w[i + 46].b2 = 0;
                            class_w[i + 46].b3 = 0;
                        }
                    }
                }
            }
            if (i == n) {
                if (class_w[k + 107].b2 == 0) {
                    class_w[i + 46].b2 = 1;
                    class_w[i + 46].b3 = 164;
                }
            }
        }
        x += 32;
        y += 32;
    }
}
