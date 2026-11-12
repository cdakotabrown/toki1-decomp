/* @SymbolStart: 0x80132FE8 */
/* @SymbolSize: 908 */
/* @PostProc: patch_stack_complete --old 32 --new 64 */
/* @PostProc: remap_stack_slots --map '24:48,28:44' */
/* Address: 0x80132FE8 (cross-prologue head) .. 0x80133370 */
/* Size: 908 bytes (227 insns incl. the 2 hoisted head words) */
#include "types.h"

typedef struct {
    u8  unk00[2];
    u8  f2;
    u8  unk03[111];
    s16 f114;
    u8  unk116[14];
    s16 f130;
} Ent;

extern Ent *taco_actor_rec;
extern u8  taco_rensya_flag;
extern u8  taco_shot_btn;
extern u8  taco_burst_btn;
extern u32 pad_data;
extern u32 pad_trig;
extern u32 frame_counter;

extern void func_8013240C(void);
extern void taco_chase(void);
extern void taco_actor(void);
extern void taco_effect(void);
extern void taco_splash_spawn(s32);
extern void taco_pair_spawn(s32);
extern void taco_effect4_spawn(s32);
extern void taco_actor_chase_spawn(s32);
extern void taco_actor_spawn(s32);
extern void taco_throw(s32);
extern void taco_hit_all(void);
extern void taco_spark(void);
extern void taco_spark2(void);
extern void taco_shot(void);
extern void taco_item_collect(void);
extern void taco_item_particle_step(void);

void taco_play(void)
{
    Ent *p;
    s32 flags;
    u32 st;
    u32 pad;
    u32 d;
    s32 sel;

    p = taco_actor_rec;
    if ((p->f2 & 0x40) != 0) {
    } else {
        func_8013240C();
        taco_chase();
        taco_actor();
        taco_effect();
        flags = 0;
        d = 3;
        st = pad_trig;
        if (taco_rensya_flag != 0) {
            pad = pad_data;
            if (pad & 0x20) {
                if (taco_shot_btn % d == 0 && (frame_counter & 3) == 0) {
                    flags = 32;
                }
            }
            if (pad & 0x40) {
                if (taco_shot_btn % d == 1 && (frame_counter & 3) == 0) {
                    flags |= 0x20;
                }
            }
            if (pad & 0x80) {
                if (taco_shot_btn % d == 2 && (frame_counter & 3) == 0) {
                    flags |= 0x20;
                }
            }
        } else {
            if (st & 0x20) {
                if (taco_shot_btn % d == 0) {
                    flags = 32;
                }
            }
            if (st & 0x40) {
                if (taco_shot_btn % d == 1) {
                    flags |= 0x20;
                }
            }
            if (st & 0x80) {
                if (taco_shot_btn % d == 2) {
                    flags |= 0x20;
                }
            }
        }
        if (st & 0x40) {
            if (taco_burst_btn % d == 0) {
                flags |= 0x40;
            }
        }
        if (st & 0x80) {
            if (taco_burst_btn % d == 1) {
                flags |= 0x40;
            }
        }
        if (st & 0x20) {
            if (taco_burst_btn % d == 2) {
                flags |= 0x40;
            }
        }
        sel = p->f114;
        if (sel == 0) {
            taco_splash_spawn(0);
        } else if (sel == 1) {
            taco_pair_spawn(0);
        } else {
            taco_effect4_spawn(0);
        }
        sel = p->f130;
        if (sel == 0) {
            taco_actor_chase_spawn(0);
        } else if (sel == 1) {
            taco_actor_spawn(0);
        }
    }
    taco_throw(flags);
    taco_hit_all();
    taco_spark();
    taco_spark2();
    taco_shot();
    taco_item_collect();
    taco_item_particle_step();
}
