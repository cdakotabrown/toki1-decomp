/* @SymbolSize: 700 */
/* Address: 0x80141CF0 */
/* Size: 700 bytes (175 insns) */
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
extern volatile u8 taco_throw_tbl_0_timer;
extern u8 *taco_actor_rec;
extern s32 frame_counter;
extern s32 taco_burst_flag;
extern s32 taco_burst_cnt;

extern void taco_actor_countdown();
extern void taco_aim_hit();
extern void sd_cmd_limit_send(s32, s32);
extern s32 taco_throw_check(s32);
extern s32 taco_sprite(void);
extern void taco_rank_clear(void);
extern s32 taco_burst_seq(void);

s32 taco_throw(s32 arg0)
{
    Sprite80142280 *e;
    s16 *p;
    u8 *q;
    s32 idx;
    u32 t;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 i;

    if (taco_throw_check(arg0) == 0) {
        return 0;
    }

    if (taco_throw_check(arg0) != 4) {
        if (taco_throw_check(arg0) != 3) {
            if (taco_throw_check(arg0) == 1) {
                if ((frame_counter & 3) == 0) {
                    if (taco_burst_cnt >= 65) {
                        taco_actor_countdown();
                        sd_cmd_limit_send(1, 1285);
                    }
                }
            } else {
                t = taco_throw_tbl_0_timer;
                if ((t & 3) == 0) {
                    idx = (s32)(t >> 2);
                    if ((u32)t >= 129U) {
                        taco_aim_hit();
                        sd_cmd_limit_send(1, 1284);
                        t = taco_throw_tbl_0_timer;
                        idx = (s32)(t >> 2);
                    }
                    if (idx < 16) {
                        e = &taco_throw_tbl[idx];
                        e->flags = 144;
                        e->x = ((s16 *)taco_actor_rec)[58];
                        e->y = (s16)(((s16 *)taco_actor_rec)[59] + 16);
                        e->z = (s16)(((s16 *)taco_actor_rec)[60] - 100);
                        e->dx = 0;
                        e->dy = 0;
                        e->dz = -32;
                        e->timer = 0;
                    }
                    p = (s16 *)taco_actor_rec;
                    dx = p[58] - p[602];
                    dy = p[59] - p[603];
                    dz = (p[60] - p[604]) - 512;
                    p = (s16 *)taco_actor_rec;
                    p[602] = (s16)(p[602] + dx / 2);
                    p = (s16 *)taco_actor_rec;
                    p[603] = (s16)(p[603] + dy / 2);
                    p = (s16 *)taco_actor_rec;
                    p[604] = (s16)(p[604] + dz / 2);
                }
            }
        }
    }

    if (taco_burst_flag != 0) {
        if (taco_burst_seq() == 0) {
            q = taco_actor_rec;
            q[2] = (u8)(q[2] & ~0xC0);
            taco_actor_countdown();
            taco_rank_clear();
        }
    } else {
        if (taco_sprite() == 0) {
            q = taco_actor_rec;
            q[2] = (u8)(q[2] & ~0xC0);
            for (i = 0; i < 16; i++) {
                taco_throw_tbl[i].flags = 0;
            }
            taco_aim_hit();
            taco_rank_clear();
        }
    }
}
