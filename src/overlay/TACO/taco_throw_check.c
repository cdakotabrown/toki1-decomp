/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x24 */
/* Address: 0x801420F0 */
/* Size: 400 bytes (100 insns) */
#include "types.h"

typedef struct
{
    s16 x;
    s16 y;
    s16 z;
    s16 pad6;
    s16 dx;
    s16 dy;
    s16 dz;
    s16 padE;
    s16 pad10;
    u8 timer;
    u8 flags;
} Sprite80142280;

extern Sprite80142280 taco_throw_tbl[];
extern u8 *taco_actor_rec;
extern s32 taco_burst_flag;

extern void taco_sprite_init();
extern void taco_pickup_clear();
extern void sd_cmd_limit_send(s32, s32);
extern s32 get_taco_rank(void);

s32 taco_throw_check(s32 arg0)
{
    Sprite80142280 *e;
    Sprite80142280 *end;
    u8 *p;
    s16 *q;

    e = taco_throw_tbl;
    end = &taco_throw_tbl[16];
    do {
        if ((e->flags & 0x80) != 0) {
            return -1;
        }
        e++;
    } while (e < end);

    if (taco_burst_flag != 0) {
        return 1;
    }

    p = taco_actor_rec;
    if ((p[2] & 0x80) != 0) {
        return 0;
    }
    if ((arg0 & 0x40) != 0) {
        if (get_taco_rank() == 3) {
            p = taco_actor_rec;
            p[2] = (u8)(p[2] | 0xC0);
            taco_sprite_init();
            taco_pickup_clear();
            taco_burst_flag = 1;
            return 3;
        }

        if (get_taco_rank() == 4) {
            p = taco_actor_rec;
            p[2] = (u8)(p[2] | 0xC0);
            taco_sprite_init();
            taco_pickup_clear();
            q = (s16 *)taco_actor_rec;
            taco_throw_tbl[0].flags = 144;
            taco_throw_tbl[0].x = q[58];
            taco_throw_tbl[0].y = q[59];
            taco_throw_tbl[0].z = q[60] - 200;
            taco_throw_tbl[0].dx = 0;
            taco_throw_tbl[0].dy = 0;
            taco_throw_tbl[0].dz = -8;
            taco_throw_tbl[0].timer = 0;
            sd_cmd_limit_send(1, 1285);
            return 4;
        }
    }

    return 0;
}
