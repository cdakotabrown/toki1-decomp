/* @CompileEnv: IDO52_FORCE_WEB_COLOR=56:s2,51:s3,52:s4,53:s5,54:s6,55:s7 */
/* @CompileFlags: -cckr */
/* obj_pool_step - TT overlay, Address: 0x80134BBC, Size: 336 bytes (84 insns) */
#include "types.h"

struct Obj {
    unsigned char active;
    unsigned char pad1;
    unsigned short type;
    unsigned char pad4[52];
    unsigned char f56[30];
    unsigned char f86;
    unsigned char f87;
    unsigned char pad88[16];
};

extern struct Obj *tt_shot_buf;
extern unsigned char *tt_game_wk;

void obj_pool_step()
{
    struct Obj *p;
    int i;
    unsigned char *q;
    unsigned char *r;

    p = tt_shot_buf;
    for (i = 0; i < 128; i++, p++) {
        if (p->active) {
            if (p->type == 64) { }
            if (p->type == 173) {
                sparkle(p);
            } else if (p->type == 172) {
                tt_shot_heading_step(p);
            }
            obj_move(p);
            if (p->type == 171) {
                q = (unsigned char *)p;
                r = tt_game_wk;
                q[62] = q[65] / 12 + 15;
                q[62] += (*(unsigned short *)(r + 10) & 1);
                q[65]++;
                if (q[65] >= 49) {
                    q[65] = 48;
                }
            } else {
                tt_anim(p->f56, &p->f86, &p->f87);
            }
        }
    }
}
