/* @CompileFlags: -cckr */
/* @RodataBase: 0x8015D9D0 */
/* @CompileEnv: IDO52_AS1_TIE_TAKE=119,172 IDO52_UGEN_TEMP_REG=35:2 IDO52_UGEN_TEMP_REG_INPLACE=occ:35 IDO52_LDA_NOINREG_CLASSES=4:2:2 IDO52_FORCE_WEB_COLOR_NOSPLIT=12,208 IDO52_FORCE_WEB_COLOR=208:s3 */
/* Address: 0x8013C570 */
#include "types.h"

typedef struct {
    s16 f0;
    s16 f2;
    s16 f4;
    u8 pad6[11];
    u8 f17;
    u8 pad18;
    u8 f19;
} TacoItem;

typedef struct {
    u8 pad0[114];
    s16 f114;
    s16 f116;
    s16 f118;
    s16 f120;
    u8 pad122[2];
    s16 f124;
    s16 f126;
    s16 f128;
    s16 f130;
    u8 pad132;
    u8 f133;
    u8 pad134[1068];
    s16 f1202;
    s16 f1204;
    s16 f1206;
    s16 f1208;
    u8 pad1210[8];
    s16 f1218;
    u8 pad1220;
    u8 f1221;
} TacoWorld;

extern TacoWorld *taco_actor_rec;
extern TacoItem taco_item_pool[];
extern u8 taco_bonus_timer;
extern s32 taco_bonus_score;
extern s32 taco_score;

extern s32 abs();
extern s32 rand();
extern void taco_sprite_flag_clear();
extern void sd_cmd_limit_send();

taco_item_collect()
{
    s32 unused0;
    s32 unused1;
    s32 unused2;
    s32 unused3;
    TacoItem *p;
    s32 i;
    s32 sel;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 kind;
    s32 bonus;
    s32 hitB;
    s32 unused4;
    s32 hitD;
    s32 hitC;
    s32 gate;
    gate = 1;
    hitB = -7;
    hitD = -7;
    hitC = -7;
    kind = -7;

    for (p = taco_item_pool; p < &taco_item_pool[16]; p++) {
        if ((p->f19 & 0x80) == 0) {
            continue;
        }
        dx = abs(p->f0 - taco_actor_rec->f116);
        dy = abs(p->f2 - taco_actor_rec->f118);
        dz = abs(p->f4 - taco_actor_rec->f120);
        if (dz >= 512 || dy >= 512 || dx >= 512) {
            continue;
        }
        taco_score = taco_score + 1000;
        sel = p->f17;
        p->f19 = 0;
        switch (sel) {
        case 0:
        case 1:
            kind = sel;
            break;
        case 2:
            hitB = 1;
            break;
        case 3:
        case 4:
            sd_cmd_limit_send(1, 1289);
            if (taco_actor_rec->f124 == 0) {
                taco_actor_rec->f124 = p->f17;
            } else if (taco_actor_rec->f126 == 0) {
                taco_actor_rec->f126 = p->f17;
            } else {
                taco_actor_rec->f128 = p->f17;
            }
            break;
        case 5:
            hitC = 1;
            if (taco_actor_rec->f133 < 239U) {
                taco_actor_rec->f133 = taco_actor_rec->f133 + 16;
            } else {
                taco_actor_rec->f133 = 255;
            }
            break;
        case 6:
            taco_bonus_timer = 32;
            hitD = 1;
            bonus = 1;
            taco_bonus_score = bonus;
            switch (rand() & 7) {
            case 0:
                bonus = 100;
                break;
            case 1:
                bonus = 200;
                break;
            case 2:
                bonus = 400;
                break;
            case 3:
                bonus = 800;
                break;
            case 4:
                bonus = 1600;
                break;
            case 5:
                bonus = 3200;
                break;
            case 6:
                bonus = 6400;
                break;
            case 7:
            default:
                bonus = 57300;
                break;
            }
            taco_score = taco_score + bonus;
            taco_bonus_score = bonus;
            break;
        case 7:
            kind = -1;
            break;
        }
        if (taco_score >= 10000000) {
            taco_score = 9999999;
        }
    }

    for (i = 0; i < 16; i++) {
        p = &taco_item_pool[i];
        if ((p->f19 & 0x80) == 0) {
            continue;
        }
        if (p->f17 == 3 || p->f17 == 4) {
            continue;
        }
        dx = abs(p->f0 - taco_actor_rec->f1204);
        dy = abs(p->f2 - taco_actor_rec->f1206);
        dz = abs(p->f4 - taco_actor_rec->f1208);
        if (dz >= 512 || dy >= 512 || dx >= 512) {
            continue;
        }
        sel = p->f17;
        p->f19 = 0;
        switch (sel) {
        case 0:
        case 1:
        case 7:
            taco_actor_rec->f1202 = sel;
            break;
        case 2:
            if (taco_actor_rec->f1218 == -1) {
                taco_actor_rec->f1218 = 1;
            }
            taco_actor_rec->f1218 = 1 - taco_actor_rec->f1218;
            break;
        case 5:
            if (taco_actor_rec->f1221 < 239U) {
                taco_actor_rec->f1221 = taco_actor_rec->f1221 + 16;
            } else {
                taco_actor_rec->f1221 = 255;
            }
            break;
        }
    }

    if (kind != taco_actor_rec->f114 && kind != -7) {
        taco_actor_rec->f114 = kind;
        switch (kind) {
        case -1:
            taco_sprite_flag_clear(0);
            sd_cmd_limit_send(1, 1280);
            gate = 0;
            break;
        case 0:
            taco_sprite_flag_clear(0);
            sd_cmd_limit_send(1, 1300);
            gate = 0;
            break;
        case 1:
            taco_sprite_flag_clear(0);
            sd_cmd_limit_send(1, 1298);
            gate = 0;
            break;
        }
    }

    if (hitB == 1) {
        if (taco_actor_rec->f130 == -1) {
            taco_actor_rec->f130 = 1;
        }
        taco_actor_rec->f130 = 1 - taco_actor_rec->f130;
        if (gate != 0) {
            switch (taco_actor_rec->f130) {
            case 0:
                sd_cmd_limit_send(1, 1297);
                gate = 0;
                break;
            case 1:
                sd_cmd_limit_send(1, 1302);
                gate = 0;
                break;
            }
        }
    }

    if (hitC == 1 && gate != 0) {
        sd_cmd_limit_send(1, 1296);
        gate = 0;
    }

    if (hitD == 1 && gate != 0) {
        sd_cmd_limit_send(1, 1289);
    }
}
