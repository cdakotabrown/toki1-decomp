/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:9 */
/* @PostProc: patch_stack_complete --old 56 --new 64 */
/* @PostProc: remap_stack_slots --from 28 --to 40 --from 32 --to 44 --from 40 --to 52 --from 44 --to 56 */
/* Address: 0x80140274 */
/* Size: 408 bytes (102 insns) */
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
    u8 pad10;
    u8 pad11;
    u8 timer;
    u8 flags;
} TacoSpr;

extern TacoSpr taco_actor_buf[];
extern TacoSpr taco_actor2_buf[];
extern u8 *taco_actor_rec;

extern void sd_cmd_limit_send(s32, s32);
extern s32 rsin(s32);
extern s32 rcos(s32);

s32 taco_actor_spawn(s32 side, s32 mask)
{
    TacoSpr *base;
    s32 odd;
    s32 i;
    s16 *act;
    s32 v;
    TacoSpr *p;

    if (side == 0) {
        act = (s16 *)taco_actor_rec;
        base = taco_actor_buf;
    } else {
        act = (s16 *)(taco_actor_rec + 1088);
        base = taco_actor2_buf;
    }

    i = 0;
    if ((mask & 0x20) != 0) {
        p = base;
        for (; i < 16; i++) {
            if ((p->flags & 0x80) == 0) {
                break;
            }
            p++;
        }
        if (i >= 16) {
            return -1;
        }

        if (side == 0) {
            sd_cmd_limit_send(1, 1283);
        }

        p->flags = 146;
        odd = i & 1;
        p->x = act[58];
        p->y = (s16)((act[59] - odd * 96) + 64);
        p->z = act[60];
        v = rsin(act[52]);
        p->dx = (s16)(v / 256);
        if (odd != 0) {
            v = rcos(act[52]);
            p->dy = (s16)((-v) / 256);
        } else {
            v = rcos(act[52]);
            p->dy = (s16)(v / 256);
        }
        p->dz = -64;
        p->timer = 0;
        p->pad11 = 0;
        return 1;
    }
    return 0;
}
