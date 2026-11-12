/* Address: 0x80085D54 */
/* Size: 408 bytes (102 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8 kind;
    u8 arg;
    u8 val;
    u8 res;
    u8 d[4];
} Log;

typedef struct {
    u8 pad0[62];
    u8 d[4];
    u8 pad66[1820];
    Log log[256];
    u8 n;
    u8 k;
} Save;

extern Save screen_w;
extern s32 rec_res;

event_log_add(val, arg)
    u8 val;
    u8 arg;
{
    s32 r;

    r = rec_res;
    screen_w.log[screen_w.n].kind = screen_w.k;
    screen_w.log[screen_w.n].val = val;
    screen_w.log[screen_w.n].arg = arg;
    if (r == 0 || r >= 7) {
        screen_w.log[screen_w.n].res = get_g_zyotai_h(screen_w.k);
    } else {
        screen_w.log[screen_w.n].res = r - 1;
    }
    rec_res = 0;
    r = 0;
    if (screen_w.k == 12) {
        screen_w.log[screen_w.n].res = 0;
    }
    screen_w.log[screen_w.n].d[0] = screen_w.d[0];
    screen_w.log[screen_w.n].d[1] = screen_w.d[1];
    screen_w.log[screen_w.n].d[2] = screen_w.d[2];
    screen_w.log[screen_w.n].d[3] = screen_w.d[3];
    screen_w.n++;
    if (screen_w.n >= 255) {
        screen_w.n = 254;
    }
}
