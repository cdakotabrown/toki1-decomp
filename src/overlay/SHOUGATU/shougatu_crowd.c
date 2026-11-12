/* @PostProc: swap_instructions --offset1 0x254 --offset2 0x258 */
/* @PostProc: swap_instructions --offset1 0x30c --offset2 0x310 */
/* @PostProc: swap_instructions --offset1 0x318 --offset2 0x31c */
/* @PostProc: patch_insn_reg_field 0xd8:rt:t4:t5 0xe4:rt:t4:t5 0x104:rt:t9:t0 0x108:rd:t5:t6 0x10c:rt:t5:t6 0x114:rt:t6:t7 0x118:rs:t6:t7 0x120:rs:t6:t7 0x120:rt:t6:t7 0x124:rd:t7:t8 0x124:rt:t6:t7 0x128:rs:t7:t8 0x128:rt:t8:t9 0x12c:rs:t8:t9 0x130:rt:t7:t8 0x13c:rt:t9:t0 0x154:rt:t1:t2 0x158:rs:t1:t2 0x158:rt:t1:t2 0x15c:rd:t0:t1 0x160:rd:t3:v1 0x160:rs:t0:t1 0x160:rt:t1:t2 0x168:rt:t2:t3 0x16c:rs:t2:t3 0x174:rs:t2:t3 0x174:rt:t2:t3 0x178:rs:t2:t3 0x180:rs:t3:v1 0x1b4:rd:t3:v1 0x1d0:rs:t3:v1 0x1d4:rs:t3:v1 0x1d8:rt:t5:t4 0x1ec:rt:t4:t3 0x1f0:rs:t5:t4 0x1f0:rt:t4:t3 0x1f4:rs:t4:t3 0x204:rs:t4:t3 0x20c:rs:t5:t4 0x21c:rd:t7:t6 0x224:rt:t7:t6 0x22c:rd:t6:t5 0x230:rt:t6:t5 0x238:rt:t8:t7 0x23c:rs:t8:t7 0x23c:rt:t8:t7 0x248:rs:t8:t7 0x254:rd:t9:t8 0x264:rs:t9:t8 0x288:rd:t5:t4 0x28c:rs:t5:t4 0x290:rs:t5:t4 0x294:rd:t3:t9 0x298:rt:t3:t9 0x2c4:rt:t3:t9 0x2c8:rs:t3:t9 0x2c8:rt:t3:t9 0x2cc:rd:t9:t8 0x2d0:rs:t9:t8 0x2d0:rt:t3:t9 0x2d8:rt:t6:t5 0x2dc:rs:t6:t5 0x2dc:rt:t6:t5 0x2e4:rs:t6:t5 0x2ec:rt:t9:t8 0x2f0:rd:t4:t3 0x2f4:rt:t4:t3 0x2f8:rt:t7:t6 0x308:rt:t6:t5 0x30c:rt:t4:t5 0x314:rt:t6:t3 0x318:rs:t6:t3 0x318:rt:t6:t3 0x31c:rs:t4:t5 0x31c:rt:t4:t5 0x320:rs:t9:t8 0x320:rt:t9:t8 0x324:rd:t8:t7 0x324:rs:t7:t6 0x328:rt:t8:t7 0x338:rt:t4:t5 0x33c:rt:t6:t3 0x340:rt:t9:t8 0x358:rt:t5:t4 0x35c:rt:t5:t4 0x364:rt:t7:t6 0x368:rt:t3:t9 0x36c:rd:t8:t7 0x36c:rs:t7:t6 0x370:rt:t6:t5 0x374:rt:t8:t7 0x378:rs:t3:t9 0x378:rt:t8:t7 0x37c:rt:t4:t3 0x384:rd:t5:t4 0x384:rs:t2:v1 0x384:rt:v1:t2 0x388:rs:t6:t5 0x38c:rt:t5:t4 0x394:rt:t4:t3 0x3ac:rt:t7:t0 0x3b0:rt:t7:t0 0x3b4:rd:t8:t6 0x3b8:rs:t8:t6 0x3b8:rt:t9:t7 0x3bc:rt:t9:t7 0x3c0:rt:t3:t8 0x3c8:rt:t3:t8 0x3cc:rt:t0:t9 0x3d4:rt:t0:t9 0x3f4:rt:t5:t4 0x3f8:rs:t5:t4 0x3f8:rt:t5:t4 0x400:rs:t5:t4 0x404:rt:t9:t8 0x40c:rt:t4:t3 0x414:rs:t4:t3 0x418:rt:t6:t5 0x428:rt:t6:t5 0x430:rt:t7:t6 0x434:rs:t7:t6 0x43c:rs:t7:t6 0x43c:rt:t7:t6 0x440:rs:t7:t6 0x440:rt:t8:t7 0x448:rt:t8:t7 0x458:rt:t9:t8 0x460:rt:t3:t9 0x464:rs:t3:t9 0x46c:rs:t3:t9 0x46c:rt:t3:t9 0x470:rs:t3:t9 0x47c:rt:t6:t5 0x48c:rd:t5:t4 0x490:rs:t5:t4 0x490:rt:t4:t3 0x494:rs:t6:t5 0x494:rt:t4:t3 0x49c:rs:t4:t3 0x4ac:rs:t4:t3 0x4b4:rs:t6:t5 0x4c0:rd:t7:t6 0x4c4:rt:t7:t6 0x4d4:rd:t8:t7 0x4d8:rt:t8:t7 */
/* @CompileEnv: IDO52_PRECOLOR_LIVEIN=a0:0 IDO52_PRECOLOR_WEB=t3:51 */
/* Address: 0x80142950 */
/* Size: 1300 bytes (325 instructions) */
#include "types.h"
#include "tokimeki/sprite.h"

typedef struct {
    s32 unk00;
    s16 unk04;
    s16 unk06;
    s16 unk08;
    s16 unk0A;
    s16 unk0C;
    u8  unk0E;
    u8  unk0F;
    s16 unk10;
    s16 unk12;
    u8  unk14;
    u8  unk15;
    u8  unk16;
    u8  unk17;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s32 unk20;
} ShougatuObj;

typedef struct {
    s16 x;
    s16 y;
} ShougatuPt;

extern s32 rand(void);

extern u8 sprite_state[];
extern u8 sprite_state3[];
extern ShougatuObj Sprites[];
extern s32 date_sprite_on;
extern u8 fade_bri;
extern s32 shougatu_k_speed_wk[];
extern s32 shougatu_crowd_vel[];
extern ShougatuPt shougatu_crowd_pos[];
extern s32 shougatu_crowd_goal[];
extern s16 shougatu_crowd_pace[];
extern volatile s16 shougatu_crowd_cnt;
extern u32 shougatu_crowd_timer;

s32 shougatu_crowd(void)
{
    s32 n;
    s16 i;
    s32 t;
    s32 m;
    s32 r;
    s32 v;

    n = shougatu_crowd_cnt;
    if (n == 0) {
        return 0;
    }

    if (shougatu_crowd_timer == 30) {
        n = (s16)(n + 1);
        ((SPRITE_REC *)Sprites)[4].x = 180;
        shougatu_crowd_pace[0] = 1;
    } else if (shougatu_crowd_timer == 90) {
        n = (s16)(n + 1);
        ((SPRITE_REC *)Sprites)[4].x = 40;
    } else if (shougatu_crowd_timer >= 201 && shougatu_crowd_timer % 20 == 0 && n < 50) {
        shougatu_crowd_cnt = n + 1;
        r = rand();
        n = shougatu_crowd_cnt;
        Sprites[n + 3].unk0E = (r % 2) * 8;
        if (Sprites[n + 3].unk0E == 8) {
            sprite_state3[n] = 12;
            shougatu_crowd_cnt = n;
            r = rand();
            n = shougatu_crowd_cnt;
            shougatu_crowd_pace[n - 1] = r % 2 + 3;
        } else {
            sprite_state3[n] = 9;
            shougatu_crowd_cnt = n;
            r = rand();
            n = shougatu_crowd_cnt;
            shougatu_crowd_pace[n - 1] = r % 2 + 1;
        }
        t = shougatu_crowd_pace[n - 1];
        shougatu_k_speed_wk[n] = 0x10000 / (t + (t >= 3) * 2 + 1);
    }

    if (date_sprite_on == 0) {
        shougatu_crowd_cnt = n;
        m = (s16)n;
    } else {
        shougatu_crowd_cnt = n;
        m = 10;
        for (i = 14; i < 54; i++) {
            Sprites[i].unk00 |= 0x80000000;
        }
    }

    for (i = 4; i < m + 4; i++) {
        Sprites[i].unk14 = Sprites[i].unk15 = Sprites[i].unk16 = fade_bri;
        Sprites[i].unk00 &= 0x7FFFFFFF;
        if (shougatu_crowd_timer % shougatu_crowd_pace[i - 4] == 0) {
            Sprites[i].unk06++;
        }
        v = shougatu_crowd_vel[i - 4];
        *(s32 *)&shougatu_crowd_pos[i - 4] = *(s32 *)&shougatu_crowd_pos[i - 4] + v;
        shougatu_crowd_vel[i - 4] = v + ((shougatu_crowd_goal[i - 4] - *(s32 *)&shougatu_crowd_pos[i - 4]) >> 8);
        Sprites[i].unk04 = shougatu_crowd_pos[i - 4].y;
        if (Sprites[i].unk06 >= 41) {
            r = rand() % 320;
            shougatu_crowd_pos[i - 4].x = 0;
            Sprites[i].unk06 = -120;
            Sprites[i].unk04 = r - 159;
            shougatu_crowd_pos[i - 4].y = Sprites[i].unk04;
            shougatu_crowd_goal[i - 4] = *(s32 *)&shougatu_crowd_pos[i - 4];
            Sprites[i].unk0E = (rand() % 2) * 8;
            if (date_sprite_on == 1) {
                Sprites[i].unk0E = 0;
            }
            if (Sprites[i].unk0E == 8) {
                sprite_state[i] = 12;
                shougatu_crowd_pace[i - 4] = rand() % 2 + 3;
            } else {
                sprite_state3[i - 3] = 9;
                shougatu_crowd_pace[i - 4] = rand() % 2 + 1;
            }
            t = shougatu_crowd_pace[i - 4];
            shougatu_crowd_vel[i - 4] = 0x10000 / (t + (t >= 3) * 2 + 1);
        }
    }

    shougatu_crowd_timer = shougatu_crowd_timer + 1;
}
