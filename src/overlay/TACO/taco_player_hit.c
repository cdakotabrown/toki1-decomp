/* @CompileFlags: -cckr */
/* taco_player_hit - TACO overlay, Address: 0x8013C2C8, Size: 680 bytes (170 insns) */
#include "types.h"

extern int taco_hit_flag;
extern unsigned char *taco_actor_rec;

taco_player_hit()
{
    int i;
    int ax;
    int ay;
    int az;
    int thr;
    unsigned int cnt;
    unsigned int c2;

    taco_hit_flag = 0;
    if (!(taco_actor_rec[2] & 0x80)) {
        goto L1;
L1:
        for (i = 16; i < 63; i++) {
            if ((taco_actor_rec[i * 136 + 1] & 0xC0) && taco_actor_rec[i * 136 + 133] != 0) {
                ax = abs(*(short *)(i * 136 + taco_actor_rec + 116) - *(short *)(taco_actor_rec + 116));
                ay = abs(*(short *)(i * 136 + taco_actor_rec + 118) - *(short *)(taco_actor_rec + 118));
                az = abs(*(short *)(i * 136 + taco_actor_rec + 120) - *(short *)(taco_actor_rec + 120));
                thr = taco_actor_rec[i * 136 + 132] << 4;
                if (az < 256 && ay < thr && ax < thr) {
                    sd_cmd_limit_send(1, 1286);
                    taco_actor_rec[2] |= 4;
                    cnt = taco_actor_rec[133];
                    if (cnt >= 13) {
                        taco_actor_rec[133] = cnt - 12;
                    } else {
                        taco_actor_rec[133] = 0;
                    }
                    *(short *)(taco_actor_rec + 122) = 16;
                    break;
                }
            }
        }
    }
    if (!(taco_actor_rec[1090] & 0x80)) {
        goto L2;
L2:
        for (i = 0; i < 16; i++) {
            if ((taco_actor_rec[i * 136 + 1] & 0xC0) && taco_actor_rec[i * 136 + 133] != 0) {
                ax = abs(*(short *)(i * 136 + taco_actor_rec + 116) - *(short *)(taco_actor_rec + 1204));
                ay = abs(*(short *)(i * 136 + taco_actor_rec + 118) - *(short *)(taco_actor_rec + 1206));
                az = abs(*(short *)(i * 136 + taco_actor_rec + 120) - *(short *)(taco_actor_rec + 1208));
                thr = taco_actor_rec[i * 136 + 132] << 4;
                if (az < 256 && ay < thr && ax < thr) {
                    taco_actor_rec[1090] |= 4;
                    c2 = taco_actor_rec[1221];
                    if (c2 >= 13) {
                        taco_actor_rec[1221] = c2 - 12;
                    } else {
                        taco_actor_rec[1221] = 0;
                    }
                    *(short *)(taco_actor_rec + 1210) = 16;
                    break;
                }
            }
        }
    }
}
