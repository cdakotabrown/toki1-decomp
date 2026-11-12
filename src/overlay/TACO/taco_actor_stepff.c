/* @CompileEnv: IDO52_UGEN_TEMP_REG=1:a0,2:t7,3:t8,4:t9,5:t0,6:t1,7:t2,8:t3,9:t4,10:t5,11:t7,12:t8,13:t9,14:t0 IDO52_UGEN_TEMP_REG_INPLACE=occ:1 */
/* @CompileFlags: -Wc,-pic0 */
/* @RodataBase: 0x8015E208 */
/* Address: 0x8015C6F8, real size 1340 bytes (335 instructions). */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 w;
} Vec3;

typedef struct {
    u8   pad00;
    u8   f01;
    u8   pad02[98];
    s16  f100;
    s16  f102;
    s16  f104;
    u16  f106;
    u8   pad108[8];
    Vec3 pos;
    Vec3 vel;
    u8   pad132[2];
    u8   f134;
    u8   pad135;
} Entry;

extern volatile s32 taco_actor_rec;
extern s32 taco_phase_cnt;

extern void srand(s32 seed);
extern s32 rand(void);
extern void taco_spark_spawn2(Vec3 a, Vec3 b, s32 c);
extern void taco_actor_disp_switch(s32 a, s32 b);
extern void taco_actor_exit(s32 a);

#define ENT  ((Entry *)(taco_actor_rec + s0))
#define ENTR ((Entry *)(s0 + taco_actor_rec))

void
taco_actor_stepff(s32 idx)
{
    Vec3 sv[2];
    Entry *p;
    s32 s0;
    s32 v0;
    s32 i;

    i = idx;
    s0 = i * 136;
    p = (Entry *)(s0 + taco_actor_rec);
    if (p->pos.w == 0) {
        if (p->f134 != 6) {
            srand(taco_phase_cnt + idx);
            ENT->f01 = 144;
            v0 = rand();
            ENT->vel.x = v0 % 40 - 20;
            v0 = rand();
            ENT->vel.y = v0 % 10 + 10;
            v0 = rand();
            ENT->vel.z = v0 % 10 + 10;
            v0 = rand();
            ENT->f106 = v0 % 6;
            p = (Entry *)(s0 + taco_actor_rec);
            taco_spark_spawn2(p->pos, p->vel, 0);
            p = (Entry *)(s0 + taco_actor_rec);
        } else {
            srand(taco_phase_cnt + idx);
            ENT->f01 = 144;
            v0 = rand();
            ENT->vel.x = v0 % 10 - 5;
            v0 = rand();
            ENT->vel.y = v0 % 2 + 2;
            v0 = rand();
            ENT->vel.z = v0 % 2 + 2;
            p = (Entry *)(s0 + taco_actor_rec);
            taco_spark_spawn2(p->pos, p->vel, 0);
            p = (Entry *)(s0 + taco_actor_rec);
        }
    }

    if (p->f134 != 6) {
        switch ((u32)p->f106) {
        case 0:
            p->f100 = p->f100 + 16;
            p = (Entry *)(s0 + taco_actor_rec);
            break;
        case 1:
            p->f102 = p->f102 + 16;
            p = (Entry *)(s0 + taco_actor_rec);
            break;
        case 2:
            p->f104 = p->f104 + 16;
            p = (Entry *)(s0 + taco_actor_rec);
            break;
        case 3:
            p->f100 = p->f100 - 16;
            p = (Entry *)(s0 + taco_actor_rec);
            break;
        case 4:
            p->f102 = p->f102 - 16;
            p = (Entry *)(s0 + taco_actor_rec);
            break;
        case 5:
            p->f104 = p->f104 - 16;
            p = (Entry *)(s0 + taco_actor_rec);
            break;
        }
        v0 = p->pos.w;
    } else {
        if (p->pos.w % 3 == 0) {
            srand(taco_phase_cnt + idx);
            v0 = rand();
            sv[1].x = v0 % 500 + ENT->pos.x - 250;
            v0 = rand();
            p = (Entry *)(taco_actor_rec + s0);
            sv[1].y = v0 % 250 + p->pos.y;
            sv[1].z = p->pos.z;
            taco_spark_spawn2(sv[1], p->vel, 0);
            p = (Entry *)(s0 + taco_actor_rec);
        }
        p->f100 = p->f100 + 2;
        p = (Entry *)(s0 + taco_actor_rec);
        p->f104 = p->f104 + 2;
        v0 = ENTR->pos.w;
    }

    if ((v0 & 1) == 0) {
        taco_actor_disp_switch(1, idx);
    } else {
        taco_actor_disp_switch(0, idx);
    }
    if (ENTR->pos.w >= 61) {
        taco_actor_exit(idx);
    }
}
