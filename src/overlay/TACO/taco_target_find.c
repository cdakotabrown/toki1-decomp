/* @CompileEnv: IDO52_FORCE_WEB_COLOR=13:a1 */
/* @SymbolSize: 776 */
/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    s32 a;
    s16 b;
    s16 c;
} TacoVec;

typedef struct {
    u8 b0;
    u8 b1;
    u8 pad2[118];
    s16 h120;
    u8 pad122[12];
    u8 b134;
    u8 b135;
} Rec;

extern u32 frame_counter;
extern Rec *taco_actor_rec;

s32 taco_target_find(out, v)
    s32 *out;
    TacoVec v;
{
    s32 i;
    s32 col;
    s32 d;
    s32 idx;
    s32 dist;
    s32 count;
    s32 best;
    Rec *q;
    s32 k;

    *out = -1;
    col = frame_counter % 48;
    count = 0;
    best = 0x9C40000;
    for (d = 0; d < 49; d++) {
        k = (col + d) % 48 + 16;
        if ((k + taco_actor_rec)->b1 & 0x80) {
            q = k + taco_actor_rec;
            if ((q->b135 & 1) && (d + taco_actor_rec)->b134 != 14) {
                count++;
                if (q->h120 < v.b && dist < best) {
                    best = dist;
                    *out = k;
                }
            }
        }
    }
    return count;
}
