/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=58:t8,68:t3,69:t4,70:t5,71:t6 */
/* shougatu_crowd_move - SHOUGATU overlay, Address: 0x80141DF0, Size: 1204 bytes */
#include "types.h"
#include "tokimeki/sprite.h"

typedef struct {
    u32 unk00;
    s16 unk04;
    s16 unk06;
    u8 unk08[6];
    u8 unk0E;
    u8 unk0F[5];
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17[13];
} Obj;

typedef struct {
    s16 lo;
    s16 hi;
} Half;

typedef union {
    s32 w;
    Half h;
} Fx;

extern u8 screen_w[];
extern u16 now_serifuev;
extern Obj Sprites[];
extern u8 fade_bri;
extern s32 shougatu_crowd_vel[];
extern Fx shougatu_crowd_pos[];
extern s32 shougatu_crowd_goal[];
extern s16 shougatu_crowd_pace[];
extern s16 shougatu_crowd_cnt;
extern u32 shougatu_crowd_timer;

shougatu_crowd_move()
{
    s16 i;
    s32 t;

    if (shougatu_crowd_cnt == 0) {
        return 0;
    }
    if (shougatu_crowd_timer == 30) {
        shougatu_crowd_cnt++;
        ((SPRITE_REC *)Sprites)[4].x = 180;
        shougatu_crowd_pace[0] = 1;
    } else if (shougatu_crowd_timer == 90) {
        shougatu_crowd_cnt++;
        ((SPRITE_REC *)Sprites)[4].x = 40;
    } else if (now_serifuev >= 6 && shougatu_crowd_timer % 20 == 0 && shougatu_crowd_cnt < 50) {
        shougatu_crowd_cnt++;
        Sprites[shougatu_crowd_cnt + 3].unk0E = rand() % 2 * 8;
        if (Sprites[shougatu_crowd_cnt + 3].unk0E == 8) {
            screen_w[shougatu_crowd_cnt + 4264] = 12;
            shougatu_crowd_pace[shougatu_crowd_cnt - 1] = rand() % 2 + 3;
        } else {
            screen_w[shougatu_crowd_cnt + 4264] = 9;
            shougatu_crowd_pace[shougatu_crowd_cnt - 1] = rand() % 2 + 1;
        }
        t = shougatu_crowd_pace[shougatu_crowd_cnt - 1];
        shougatu_crowd_vel[shougatu_crowd_cnt - 1] = 65536 / ((t >= 3) * 2 + t + 1);
    }
    for (i = 4; i < shougatu_crowd_cnt + 4; i++) {
        Sprites[i].unk14 = Sprites[i].unk15 = Sprites[i].unk16 = fade_bri;
        Sprites[i].unk00 &= 0x7FFFFFFF;
        if (shougatu_crowd_timer % shougatu_crowd_pace[i - 4] == 0) {
            Sprites[i].unk06++;
        }
        shougatu_crowd_pos[i - 4].w += shougatu_crowd_vel[i - 4];
        shougatu_crowd_vel[i - 4] += (shougatu_crowd_goal[i - 4] - shougatu_crowd_pos[i - 4].w) >> 8;
        Sprites[i].unk04 = shougatu_crowd_pos[i - 4].h.hi;
        if (Sprites[i].unk06 >= 41) {
            t = rand();
            shougatu_crowd_pos[i - 4].h.lo = 0;
            Sprites[i].unk06 = -120;
            Sprites[i].unk04 = t % 320 - 159;
            shougatu_crowd_pos[i - 4].h.hi = Sprites[i].unk04;
            shougatu_crowd_goal[i - 4] = shougatu_crowd_pos[i - 4].w;
            Sprites[i].unk0E = rand() % 2 * 8;
            if (Sprites[i].unk0E == 8) {
                screen_w[i + 4261] = 12;
                shougatu_crowd_pace[i - 4] = rand() % 2 + 3;
            } else {
                screen_w[i + 4261] = 9;
                shougatu_crowd_pace[i - 4] = rand() % 2 + 1;
            }
            t = shougatu_crowd_pace[i - 4];
            shougatu_crowd_vel[i - 4] = 65536 / ((t >= 3) * 2 + t + 1);
        }
    }
    shougatu_crowd_timer++;
}
