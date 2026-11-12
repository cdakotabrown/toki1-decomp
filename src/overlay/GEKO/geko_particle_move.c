/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=occ:1 */
/* @PostProc: remap_stack_slots --map 72:76 */
/* Address: 0x80139DCC */
/* Size: 808 bytes (202 instructions) */
#include "types.h"

typedef struct {
    s32 flags;
    s16 x;
    s16 y;
    u16 w;
    u16 h;
    s16 unk0C;
    u8  frame;
    u8  unk0F;
    u8  unk10[20];
} Spr;

extern s32 rand(void);
extern Spr Sprites[];
extern s16 geko_ptc_phase[];
extern s16 geko_ptc_sub[];

void geko_particle_move(void)
{
    s16 i;
    s16 av;
    s16 bv;

    for (i = 4; i < 19; i++) {
        Sprites[i].y += 10;

        if (((rand() & 3) == 0 && Sprites[i].y >= 17 &&
             (av = geko_ptc_phase[i - 4]) == 0 &&
             geko_ptc_sub[i - 4] == 0) ||
            (av = geko_ptc_phase[i - 4]) != 0 || geko_ptc_sub[i - 4] != 0) {
            Sprites[i].y -= 10;
            if (av == 0) {
                if (geko_ptc_sub[i - 4] == 0) {
                    if (Sprites[i].x < -87) {
                        Sprites[i].y = rand() % 4 + 28;
                    } else {
                        Sprites[i].y = rand() % 15 + 17;
                    }
                    Sprites[i].x -= 12;
                    Sprites[i].frame = 8;
                    Sprites[i].w = 24;
                    Sprites[i].h = 8;
                    geko_ptc_sub[i - 4] = geko_ptc_sub[i - 4] + 1;
                } else {
                    geko_ptc_sub[i - 4] = 0;
                    geko_ptc_phase[i - 4] = av + 1;
                }
            } else if (av == 5) {
                Sprites[i].frame = 0;
                Sprites[i].w = 8;
                Sprites[i].h = 24;
                Sprites[i].x = rand() % 167 - 159;
                if (Sprites[i].x >= -87) {
                    Sprites[i].x += 153;
                }
                Sprites[i].y = -95;
                geko_ptc_phase[i - 4] = 0;
                geko_ptc_sub[i - 4] = 0;
            } else {
                bv = geko_ptc_sub[i - 4];
                if (av == 4) {
                    if (bv == 0) {
                        Sprites[i].frame -= 24;
                        geko_ptc_sub[i - 4] = bv + 1;
                    } else {
                        geko_ptc_sub[i - 4] = 0;
                        geko_ptc_phase[i - 4] = av + 1;
                    }
                } else {
                    if (bv == 0) {
                        Sprites[i].frame += 24;
                        geko_ptc_sub[i - 4] = bv + 1;
                    } else {
                        geko_ptc_sub[i - 4] = 0;
                        geko_ptc_phase[i - 4] = av + 1;
                    }
                }
            }
        } else if (Sprites[i].y >= 17 && av == 0 && geko_ptc_sub[i - 4] == 0) {
            Sprites[i].x = rand() % 167 - 159;
            if (Sprites[i].x >= -87) {
                Sprites[i].x += 153;
            }
            Sprites[i].y = -95;
        }
    }
}
