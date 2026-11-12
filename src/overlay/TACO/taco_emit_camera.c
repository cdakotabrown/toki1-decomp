/* @CompileFlags: -Wc,-pic1 */
/* @SymbolSize: 1476 */
/* @PostProc: patch_stack_complete --old 80 --new 96 */
/* Address: 0x8015801C */
/* Size: 1476 bytes (369 instructions) */
#include "types.h"

typedef struct { short vx, vy, vz, pad; } SVECTOR;

typedef struct {
    u8  unk00[116];
    s16 f116;
    s16 f118;
} Cam;

extern s32 taco_phase_cnt;
extern s32 taco_emit_mode;
extern SVECTOR taco_zero_vec;
extern SVECTOR taco_emit_pos;
extern Cam *taco_actor_rec;

extern s32 srand(s32);
extern void taco_actor_alloc(s32, s32, SVECTOR, SVECTOR, SVECTOR, s32, s32);

void taco_emit_camera(void)
{
    SVECTOR v;
    s32 mode;
    s32 x;

    srand(taco_phase_cnt);
    mode = taco_emit_mode;
    v.vy = 2048;
    v.vx = 0;
    v.vz = 0;
    if (mode == 0) {
        x = taco_actor_rec->f116;
        if ((x < 0 && taco_actor_rec->f118 < -700) ||
            (x > 0 && taco_actor_rec->f118 > -700)) {
            v.pad = 17;
            taco_emit_mode = 1;
            taco_actor_alloc(12, 17, taco_zero_vec, taco_emit_pos, v, 2, 16);
            v.pad = 0;
            taco_actor_alloc(12, 17, taco_zero_vec, taco_emit_pos, v, 2, 16);
        } else {
            v.pad = 1;
            taco_emit_mode = 2;
            taco_actor_alloc(12, 17, taco_zero_vec, taco_emit_pos, v, 2, 16);
            v.pad = 16;
            taco_actor_alloc(12, 17, taco_zero_vec, taco_emit_pos, v, 2, 16);
        }
    } else if (mode == 1) {
        v.pad = 17;
        taco_actor_alloc(12, 17, taco_zero_vec, taco_emit_pos, v, 2, 16);
        v.pad = 0;
        taco_actor_alloc(12, 17, taco_zero_vec, taco_emit_pos, v, 2, 16);
    } else {
        v.pad = 1;
        taco_actor_alloc(12, 17, taco_zero_vec, taco_emit_pos, v, 2, 16);
        v.pad = 16;
        taco_actor_alloc(12, 17, taco_zero_vec, taco_emit_pos, v, 2, 16);
    }
}
