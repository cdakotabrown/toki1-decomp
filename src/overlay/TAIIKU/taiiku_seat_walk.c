/* @CompileEnv: IDO52_FORCE_WEB_COLOR=107:v1 */
/* @PostProc: remap_stack_slots --from 32 --to 48 */
/* @SymbolStart: 0x80137870 */
/* @SymbolSize: 1216 */
/* @CompileFlags: -cckr -Wc,-pic0 */
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
    u8 pad4[4];
    s16 h8;
    u8 padA[12];
    s16 h22;
    s16 h24;
    u8 pad1A[10];
    Fix x;
    Fix y;
    u8 pad2C[23];
    u8 b67;
} Actor;

typedef struct {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
    u8 b4;
    u8 pad5[7];
    s16 h12;
    s16 h14;
    s32 w16;
    s32 w20;
    s32 w24;
    s32 w28;
    u8 pad20[2];
    s16 h34;
} Sel;

s32 taiiku_kari_cnt = 0;
s16 taiiku_kari_btn = 0;
Sel taiiku_kari_run[4] = {0};
extern Actor class_w[];
extern s32 taiiku_konami_mode;
extern s16 taiiku_kari_scrl_hi;
extern s32 taiiku_kari_walk_cnt[];
extern s32 taiiku_kari_walk_dx[];
extern s32 taiiku_kari_walk_dy[];
extern Fix taiiku_kari_walk_ox[];
extern u8 taiiku_kari_gseat;
extern u8 taiiku_kari_girl;
extern s16 taiiku_kari_card;
extern s16 taiiku_kari_gl_card[];
extern s32 taiiku_kari_scrv;

void k_reset();

s32 taiiku_seat_walk(idx)
    s32 idx;
{
    Sel *s;
    Actor *a;
    Actor *b;
    s32 k;
    s32 c;

    s = &taiiku_kari_run[idx];
    c = s->b2;
    a = &class_w[c];
    if (a[46].b2 != 0 && (idx != 0 || taiiku_konami_mode == 255)) {
        a[46].b2 = 0;
        a[46].b3 = 0;
        if (idx == 0) {
            k = 0;
            do {
                k++;
                class_w[k + 103].b2 = 0;
                class_w[k + 103].b3 = 0;
            } while (k < 3);
            k_reset(0);
            c = s->b2;
        }
        b = &class_w[idx];
        b[107].b2 = 1;
        b[107].b3 = 164;
        b[107].x.w.hi = ((s16)c % 10) * 32 - taiiku_kari_scrl_hi + 480;
        b[107].y.w.hi = -((s16)c / 10) * 16 - 48;
        taiiku_kari_walk_dx[idx] = (b[96].x.l - b[107].x.l) >> 7;
        taiiku_kari_walk_dy[idx] = (b[96].y.l - b[107].y.l) >> 7;
        taiiku_kari_walk_ox[idx].l = b[107].x.l - b[96].x.l;
        if (taiiku_kari_walk_dx[idx] > 0) {
            b[107].b3 |= 1;
        }
        taiiku_kari_walk_cnt[idx] = 0;
        s->b3 = 0;
        s->b4 = 0;
        s->h12 = 0;
        s->h14 = 0;
        s->w16 = 0;
        s->w20 = 0;
        s->w24 = 0;
        s->w28 = 0;
        if (idx == 0) {
            taiiku_kari_run[0].h34 = -160;
            taiiku_kari_scrv = 0;
        }
        return 0;
    }
    a = &class_w[taiiku_kari_gseat];
    if (a[46].b2 != 0 && taiiku_konami_mode == 1 && idx == 0) {
        a[46].b2 = 0;
        a[46].b3 = 0;
        k = 0;
        do {
            k++;
            class_w[k + 103].b2 = 0;
            class_w[k + 103].b3 = 0;
        } while (k < 3);
        k_reset(0);
        class_w[107].h22 = class_w[115].h22;
        class_w[107].b67 = class_w[115].b67;
        class_w[107].h24 = 0;
        class_w[107].h8 = 0;
        class_w[107].b2 = 1;
        class_w[107].b3 = 164;
        class_w[107].x.w.hi = taiiku_kari_gseat * 32 - taiiku_kari_scrl_hi + 480;
        class_w[107].y.w.hi = -48;
        taiiku_kari_walk_dx[0] = (class_w[96].x.l - class_w[107].x.l) >> 7;
        taiiku_kari_walk_dy[0] = (class_w[96].y.l - class_w[107].y.l) >> 7;
        taiiku_kari_walk_ox[0].l = class_w[107].x.l - class_w[96].x.l;
        if (taiiku_kari_walk_dx[0] > 0) {
            class_w[107].b3 |= 1;
        }
        taiiku_kari_walk_cnt[0] = 0;
        taiiku_kari_run[0].b3 = 0;
        taiiku_kari_run[0].b4 = 0;
        taiiku_kari_run[0].h12 = 0;
        taiiku_kari_run[0].h14 = 0;
        taiiku_kari_run[0].w16 = 0;
        taiiku_kari_run[0].w20 = 0;
        taiiku_kari_run[0].w24 = 0;
        taiiku_kari_run[0].w28 = 0;
        taiiku_kari_run[0].h34 = -160;
        taiiku_kari_scrv = 0;
        return 0;
    }
    taiiku_kari_walk_ox[idx].l += taiiku_kari_walk_dx[idx];
    class_w[idx + 107].x.w.hi = class_w[idx + 96].x.w.hi + taiiku_kari_walk_ox[idx].w.hi;
    class_w[idx + 107].y.l += taiiku_kari_walk_dy[idx];
    if (++taiiku_kari_walk_cnt[idx] >= 128) {
        class_w[idx + 96].h24 = idx + 144;
        if (idx == 0 && taiiku_konami_mode == 1) {
            class_w[107].h24 = taiiku_kari_gl_card[taiiku_kari_girl] * 6 + 5;
        } else {
            class_w[idx + 107].h24 = taiiku_kari_card * 6 + 5;
        }
        class_w[idx + 107].x.w.hi = class_w[idx + 96].x.w.hi - 8;
        class_w[idx + 107].y.l = class_w[idx + 96].y.l;
        class_w[idx + 107].b2 = 64;
        class_w[idx + 107].b3 = 164;
        return 1;
    }
    return 0;
}
