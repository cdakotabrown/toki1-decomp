/* @PostProc: patch_insn_reg_field 0x1a4:rt:t1:t2 0x1a8:rt:t3:t1 0x1ac:rs:t1:t2 0x1b0:rd:t2:t3 0x1b0:rt:t1:t2 0x1b4:rs:t1:t2 0x1b8:rd:t2:t3 0x1bc:rs:t2:t1 0x1f0:rt:t9:t7 0x1f4:rs:t7:t8 0x1f8:rd:t8:t9 0x1f8:rt:t7:t8 0x1fc:rs:t7:t8 0x200:rd:t8:t9 0x204:rs:t8:t7 0x20c:rd:t1:t2 0x214:rd:t1:t2 0x218:rt:t1:t2 0x21c:rt:t2:t1 0x224:rs:t2:t1 0x24c:rt:at:t4 0x250:rt:at:t4 0x258:rt:at:t4 0x25c:rt:at:t4 0x26c:rt:t7:t5 0x274:rt:t7:t5 0x284:rt:t8:t6 0x288:rt:t9:t8 0x28c:rs:t8:t6 0x290:rt:t0:t7 0x294:rt:t1:t9 0x298:rt:t3:t2 0x29c:rt:t9:t8 0x2a4:rt:t0:t7 0x2a8:rs:t1:t9 0x2ac:rd:t2:t0 0x2ac:rt:t1:t9 0x2b0:rs:t1:t9 0x2b4:rd:t2:t0 0x2b8:rt:t2:t0 0x2bc:rs:t3:t2 0x2c0:rd:t4:t1 0x2c0:rt:t3:t2 0x2c4:rs:t3:t2 0x2c8:rd:t4:t1 0x2cc:rt:t4:t1 0x2d0:rt:t5:t3 0x2d4:rt:t6:t4 0x2d8:rt:t8:t6 0x2dc:rt:t9:t8 0x2e0:rt:t1:t9 0x2e4:rt:t2:t0 0x2e8:rd:t7:t5 0x2e8:rs:t5:t3 0x2e8:rt:t6:t4 0x2ec:rd:t0:t7 0x2ec:rs:t8:t6 0x2ec:rt:t9:t8 0x2f0:rd:t3:t2 0x2f0:rs:t1:t9 0x2f0:rt:t2:t0 0x2f4:rt:t7:t5 0x2f8:rt:t0:t7 0x2fc:rt:t3:t2 0x1e4:rt:t7:t8 0x1ec:rt:t7:t8 */
/* @PostProc: patch_stack_complete --old 136 --new 152 */
/* @PostProc: remap_stack_slots --map '80:104,84:108,96:120,100:124,104:128,116:140' */
/* Address: 0x8013FF38 */
#include "types.h"

typedef struct {
    s16 a;
    s16 b;
} TacoPair;

typedef struct {
    TacoPair pos;
    TacoPair alt;
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad14;
    u8 pad16;
    u8 sel;
    u8 timer;
    u8 flags;
} TacoEnt;

typedef struct {
    u8 pad0;
    u8 f1;
    u8 pad2[114];
    s16 f116;
    s16 f118;
    s16 f120;
    u8 pad122[13];
    u8 f135;
} TacoObj;

extern TacoObj *taco_actor_rec;
extern TacoEnt taco_actor_buf[];
extern TacoEnt taco_actor2_buf[];
extern s32 frame_counter;

extern void vector_normalize();
extern void taco_target_find();
extern void taco_actor_disp();

void
_taco_actor(s32 which)
{
    TacoObj *obj;
    TacoObj *e;
    TacoEnt *base;
    TacoEnt *p;
    s32 sel;
    s32 vec[5];
    s32 out[4];
    s32 i;

    if (which == 0) {
        obj = taco_actor_rec;
        base = taco_actor_buf;
    } else {
        obj = taco_actor_rec;
        base = taco_actor2_buf;
        obj += 8;
    }

    i = 0;
    p = base;
    do {
        p->timer = p->timer + 1;
        if (p->timer == 255) {
            p->flags = 0;
        }
        if (p->alt.a < -32000) {
            p->flags = 0;
        }
        if (obj->f120 + 1024 < p->alt.a) {
            p->flags = 0;
        }

        if ((p->flags & 0x80) != 0) {
            if ((p->flags & 1) != 0 && p->timer >= 17U) {
                if ((frame_counter & 3) == 0) {
                    sel = p->sel;
                    if (sel != -1) {
                        e = &taco_actor_rec[sel];
                        if (e->f120 < p->alt.a && (e->f1 & 0x80) != 0 &&
                            (e->f135 & 1) != 0) {
                            vec[0] = e->f116 - p->pos.a;
                            vec[1] = e->f118 - p->pos.b;
                            vec[2] = 0;
                            vector_normalize(vec, out);
                            p->vx = (p->vx + out[0] / 64) / 2;
                            if (p->vx >= 65) {
                                p->vx = 64;
                            }
                            p->vy = (p->vy + out[1] / 64) / 2;
                            if (p->vy >= 65) {
                                p->vx = 64;
                            }
                        }
                    } else {
                        taco_target_find(&sel, p->pos, p->alt);
                        p->sel = sel;
                    }
                }
            } else if (p->timer >= 17U && p->sel == 0) {
                p->sel = 1;
                p->timer = 0;
                p->vz = -512;
                p->vx = p->vx / 16;
                p->vy = p->vy / 16;
            }

            p->pos.a += p->vx;
            p->pos.b += p->vy;
            p->alt.a += p->vz;
            taco_actor_disp(base, i);
        }

        i++;
        p++;
    } while (i != 16);
}
