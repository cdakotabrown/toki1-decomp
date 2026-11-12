/* @CompileEnv: IDO52_LDA_NOINREG_CLASSES=4 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x2c */
/* @PostProc: patch_insn_reg_field 0x1c:rt:a1:t3 0x28:rs:a1:t3 0x28:rt:a1:t3 0x30:rt:t3:t2 0x34:rt:t2:t1 0x38:rt:t1:t0 0x3c:rt:t0:a3 0x40:rt:a3:a2 0x44:rt:a2:a1 0x5c:rs:a2:a1 0x7c:rs:a3:a2 0x150:rt:t0:a3 0x168:rt:t1:t0 0x180:rt:t2:t1 0x198:rt:t3:t2 0x210:rt:a1:t3 */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 unk6;
    s16 vx;
    s16 vy;
    s16 vz;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 kind;
    u8 tick;
    u8 phase;
} Blk;

extern Blk taco_item_pool[];
extern u8 *taco_actor_rec;

extern void taco_item_disp(s32);
extern void taco_item_disp2(s32);

void taco_item_particle_step(void)
{
    Blk *p;
    Blk *pend;
    s32 i;

    p = taco_item_pool;
    pend = &taco_item_pool[16];
    while (p < pend) {
        p->tick = p->tick + 1;
        if (p->tick == 255) {
            p->tick = 0;
            p->phase = p->phase + 1;
        }
        if (p->phase == 133) {
            p->phase = 0;
        }
        if (p->z >= 10001) {
            p->phase = 0;
        }
        if ((p->phase & 0x80) != 0) {
            p->x += p->vx;
            p->y += p->vy;
            p->z += p->vz;
            if (p->vx > 0) {
                p->vx = p->vx - 1;
            }
            if (p->vy > 0) {
                p->vy = p->vy - 1;
            }
            if (p->vx < 0) {
                p->vx = p->vx + 1;
            }
            if (p->vy < 0) {
                p->vy = p->vy + 1;
            }
            if ((p->phase & 4) == 0) {
                if (p->x > 2048) {
                    p->x = 2048;
                }
                if (p->x < -2048) {
                    p->x = -2048;
                }
                if (p->y > 128) {
                    p->y = 128;
                }
                if (p->y < -1792) {
                    p->y = -1792;
                }
                if (*(s16 *)(taco_actor_rec + 120) + 16 < p->z) {
                    if (p->vz > 0) {
                        p->vz = p->vz * -1;
                    }
                }
                if (p->z < -4096) {
                    if (p->vz < 0) {
                        p->vz = p->vz * -1;
                    }
                }
            }
        }
        p++;
    }
    p = taco_item_pool;
    i = 0;
    do {
        if ((p->phase & 0x80) != 0) {
            if ((u32)p->kind < 2U) {
                taco_item_disp(i);
            } else {
                taco_item_disp2(i);
            }
        }
        i++;
        p++;
    } while (i != 16);
}
