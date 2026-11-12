/* @CompileEnv: IDO52_AS1_NO_PASS1_PROP=12,20 */
#include "types.h"

extern u8 *taco_actor_rec;

extern s32 rand(void);
extern s32 taco_addr_check(u32 *);

void taco_list_chan_rand_set(u32 *p, u16 a1, s32 a2, s32 a3)
{
    s32 mode;
    s32 lim;
    s32 n;
    s32 c1;
    s32 c2;
    s32 c3;
    s32 cnt;
    s32 v;
    u32 dens;

    dens = taco_actor_rec[2722];
    cnt = 0;
    p += 9;
    mode = (s32)p[-8];
    lim = (s32)p[-6];
    c3 = a3;
    c2 = a2;
    c1 = a1;
    if (dens < 16U) {
        n = rand() % 150 + 100;
    } else if (dens >= 16U && dens < 32U) {
        n = rand() % 100 + 50;
    } else if (dens >= 32U && dens < 48U) {
        n = rand() % 50 + 30;
    } else if (dens >= 48U && dens < 64U) {
        n = rand() % 30 + 20;
    } else if (dens >= 64U && dens < 100U) {
        n = rand() % 10 + 5;
    }
    for (;;) {
        if (taco_addr_check(p) == 0) {
            break;
        }
        if (p[0] == 0x24000507 || p[0] == 0x34000609) {
            if ((u32)taco_actor_rec[2722] < 80U) {
                if (cnt >= n) {
                    v = (s32)p[2];
                    p[1] = (p[1] & 0xFFFF) +
                           ((c3 << 22) + (((c2 >> 4) & 0x3F) << 16));
                    p += 2;
                    p[0] = ((v & 0xFFFF) + (v & 0x01E00000)) +
                           ((c1 & 0x1F) << 16);
                    cnt = 0;
                }
                cnt++;
            } else if ((n = rand() % 70) == 0) {
                v = (s32)p[2];
                p[1] = (p[1] & 0xFFFF) +
                       ((c3 << 22) + (((c2 >> 4) & 0x3F) << 16));
                p += 2;
                p[0] = ((v & 0xFFFF) + (v & 0x01E00000)) +
                       ((c1 & 0x1F) << 16);
            }
        }
        p += 1;
        if (mode != 0 && (u32)lim < (u32)p) {
            break;
        }
        if (mode == 0 && (u32)(lim * 4 + (s32)p + 12) < (u32)p) {
            break;
        }
    }
}
