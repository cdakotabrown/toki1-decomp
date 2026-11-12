/* @SymbolSize: 500 */
/* Address: 0x80157AF0 */
/* Size: 500 bytes (125 instructions) */
/* tail section.  @SymbolSize: 500 windows this function correctly meanwhile. */
/* @PostProc: patch_stack_complete --old 80 --new 96 */
/* @PostProc: remap_stack_slots --map '60:64,64:80,66:82,68:84' */
/* @PostProc: swap_instructions --offset1 0x15c --offset2 0x160 */
#include "types.h"

typedef struct { short vx, vy, vz, pad; } SVECTOR;

typedef struct {
    u8  unk00[116];
    s16 f116;
} Cam;

extern s32 taco_phase_cnt;
extern SVECTOR taco_zero_vec;
extern Cam *taco_actor_rec;

extern void srand(s32);
extern s32 rand(void);
extern s32 taco_actor_alloc(s32, s32, SVECTOR, SVECTOR, SVECTOR, s32, s32);

void taco_emit_scatter(void)
{
    SVECTOR dir;
    SVECTOR pos;
    s32 r1;
    s32 r2;
    s32 r3;

    srand(taco_phase_cnt);
    dir.vy = 2048;
    dir.vx = 0;
    dir.vz = 0;
    dir.pad = 0;
    r1 = rand();
    r2 = rand();
    pos.vx = r2 % 5000 + taco_actor_rec->f116 + r1 % 3000 - 4000;
    r3 = rand();
    pos.vy = (r3 % 2) * 8000 - 3000;
    pos.vz = -10050;
    dir.pad = taco_actor_alloc(9, 19, taco_zero_vec, pos, dir, 64, 0);
    pos.vz = -10000;
    taco_actor_alloc(10, 20, taco_zero_vec, pos, dir, 64, 128);
}
