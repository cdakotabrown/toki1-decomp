/* @RodataBase: 0x8015D7CC */
/* @SymbolStart: 0x801433C0 */
/* @SymbolSize: 932 */
/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u8  pad0[16];
    union {
        struct {
            unsigned b0 : 1;
            unsigned b1 : 1;
            unsigned rest : 30;
        } w;
        struct {
            u8 b0;
            u8 b1;
            u8 lo : 4;
            u8 hi : 4;
            u8 b3;
        } c;
    } f;
    u8  pad20[36];
} Chr;

typedef struct {
    u8 f[8];
} Log;

typedef struct {
    u8  pad0[62];
    u8  a62;
    u8  a63;
    u8  a64;
    u8  a65;
    u8  pad66[178];
    u16 sel;
    u8  pad246[194];
    Chr chr[11];
    u8  pad1056[830];
    Log log[256];
    u8  n;
} Save;

extern Save screen_w;
extern s32 bunka_girl;

void bunkasai_event_log_add()
{
    Save *sv;
    u8 status;

    sv = &screen_w;
    switch (screen_w.sel >> 12) {
    case 0:
        if (screen_w.chr[0].f.c.hi == 0 ||
            (sv->chr[1].f.c.hi == 0 && sv->chr[1].f.w.b1 == 1)) {
            status = 2;
        } else {
            status = 1;
        }
        break;
    case 1:
        if (screen_w.chr[0].f.c.hi == 1 ||
            (sv->chr[1].f.c.hi == 1 && sv->chr[1].f.w.b1 == 1)) {
            status = 6;
        } else {
            status = 5;
        }
        break;
    case 2:
        if (screen_w.chr[0].f.c.hi == 2 ||
            (sv->chr[2].f.c.hi == 2 && sv->chr[2].f.w.b1 == 1)) {
            status = 10;
        } else {
            status = 9;
        }
        break;
    case 3:
        if (screen_w.chr[0].f.c.hi == 3 ||
            (sv->chr[2].f.c.hi == 3 && sv->chr[2].f.w.b1 == 1)) {
            status = 14;
        } else {
            status = 13;
        }
        break;
    case 4:
        if (screen_w.chr[0].f.c.hi == 7 ||
            (sv->chr[3].f.c.hi == 7 && sv->chr[3].f.w.b1 == 1)) {
            status = 18;
        } else {
            if (bunka_girl == 10 && get_g_zyotai_low(10) == 4) {
                return;
            }
            status = 17;
        }
        break;
    case 5:
        if (screen_w.chr[0].f.c.hi == 5 ||
            (sv->chr[3].f.c.hi == 5 && sv->chr[3].f.w.b1 == 1)) {
            status = 22;
        } else {
            status = 21;
        }
        break;
    }
    sv->log[screen_w.n].f[0] = bunka_girl;
    sv->log[screen_w.n].f[2] = 20;
    sv->log[screen_w.n].f[1] = status;
    sv->log[screen_w.n].f[3] = get_g_zyotai_h(bunka_girl);
    sv->log[screen_w.n].f[4] = screen_w.a62;
    sv->log[screen_w.n].f[5] = screen_w.a63;
    sv->log[screen_w.n].f[6] = screen_w.a64;
    sv->log[screen_w.n].f[7] = screen_w.a65;
    screen_w.n++;
    if (screen_w.n >= 255) {
        screen_w.n = 254;
    }
}
