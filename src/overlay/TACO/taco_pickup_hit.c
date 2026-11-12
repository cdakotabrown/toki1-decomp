/* @CompileFlags: -cckr */
#include "types.h"

struct Ent {
    short x;
    short y;
    short z;
    unsigned char pad[13];
    unsigned char flag;
};

extern struct Ent taco_pickup_tbl[];
extern int taco_hit_flag;
extern unsigned char *taco_actor_rec;

taco_pickup_hit()
{
    int i;
    int ax;
    int ay;
    int az;
    unsigned int cnt;
    unsigned int c2;

    taco_hit_flag = 0;
    if (!(taco_actor_rec[2] & 0x80)) {
        for (i = 0; i < 16; i++) {
            if (taco_pickup_tbl[i].flag & 0x80) {
                ax = abs(taco_pickup_tbl[i].x - *(short *)(taco_actor_rec + 116));
                ay = abs(taco_pickup_tbl[i].y - *(short *)(taco_actor_rec + 118));
                az = abs(taco_pickup_tbl[i].z - *(short *)(taco_actor_rec + 120));
                if (az < 256 && ay < 128 && ax < 128) {
                    taco_pickup_tbl[i].flag = 0;
                    sd_cmd_limit_send(1, 1286);
                    taco_actor_rec[2] |= 4;
                    cnt = taco_actor_rec[133];
                    if (cnt >= 5) {
                        taco_actor_rec[133] = cnt - 4;
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
        for (i = 0; i < 16; i++) {
            if (taco_pickup_tbl[i].flag & 0x80) {
                ax = abs(taco_pickup_tbl[i].x - *(short *)(taco_actor_rec + 1204));
                ay = abs(taco_pickup_tbl[i].y - *(short *)(taco_actor_rec + 1206));
                az = abs(taco_pickup_tbl[i].z - *(short *)(taco_actor_rec + 1208));
                if (az < 256 && ay < 128 && ax < 128) {
                    taco_pickup_tbl[i].flag = 0;
                    taco_actor_rec[1090] |= 4;
                    c2 = taco_actor_rec[1221];
                    if (c2 >= 5) {
                        taco_actor_rec[1221] = c2 - 4;
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
