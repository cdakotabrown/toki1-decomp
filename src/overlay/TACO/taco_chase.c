/* @RodataBase: 0x8015D4F0 */
/* @PostProc: patch_stack_complete --old 104 --new 128 */
/* @PostProc: remap_stack_slots --map '44:56,48:72,52:76,56:80,64:88,68:92,72:96,80:112,92:64' */
/* Address: 0x801328B0 */
/* Size: 1848 bytes (462 instructions) */
#include "types.h"

typedef struct TacoVec {
      s32 vx;
      s32 vy;
      s32 vz;
      s32 pad;
} TacoVec;

typedef struct TacoRec {
      u8 unk00;
      u8 anim;
      u8 state;
      u8 pad03[105];
      s16 sclX;
      s16 sclY;
      s16 sclZ;
      s16 unk72;
      s16 posX;
      s16 posY;
      s16 posZ;
      s16 unk7A;
      s16 rotX;
      s16 rotY;
      s16 velZ;
      s16 unk82;
      u8 unk84;
      u8 unk85;
      u8 unk86;
      u8 unk87;
} TacoRec;

extern TacoRec *taco_actor_rec;
extern s32 frame_counter;

extern s32 taco_count_targets(s32 *arg0);
extern void taco_splash_spawn(s32 arg0, s32 arg1);
extern void taco_pair_spawn(s32 arg0, s32 arg1);
extern void taco_effect4_spawn(s32 arg0, s32 arg1);
extern void taco_actor_chase_spawn(s32 arg0, s32 arg1);
extern void taco_actor_spawn(s32 arg0, s32 arg1);
extern u32 vector_normalize(TacoVec *arg0, TacoVec *arg1);
extern s32 rand(void);

void
taco_chase(void)
{
    TacoRec *rec;
    TacoRec *tbl;
    s32 mode;
    s32 sel;
    u32 mag;
    s32 idx;
    TacoVec src;
    TacoVec dst;
    s32 ret;

    ret = taco_count_targets(&idx);
    mode = 0;
    if (ret != 0) {
        if ((frame_counter & 7) == 0) {
            mode = 32;
        } else if ((frame_counter & 0xF) == 0) {
            mode = 32;
        }
    }

    tbl = taco_actor_rec;
    rec = &tbl[8];
    tbl[8].anim = 128;
    rec->unk87 = 1;
    rec->unk84 = 2;
    sel = rec->unk72;
    if (sel == 0) {
        taco_splash_spawn(1, mode);
    } else if (sel == 1) {
        taco_pair_spawn(1, mode);
    } else {
        taco_effect4_spawn(1, mode);
    }

    sel = rec->unk82;
    if (sel == 0) {
        taco_actor_chase_spawn(1, mode);
    } else if (sel == 1) {
        taco_actor_spawn(1, mode);
    }

    if ((u32)ret < 8) {
        tbl = taco_actor_rec;
        src.vx = tbl->posX - rec->posX;
        src.vy = tbl->posY - rec->posY;
        src.vz = 0;
        mag = vector_normalize(&src, &dst);
        switch ((u32)rec->state) {
        default:
            rec->state = 0;
        case 0:
            rec->anim = 128;
            rec->state = rec->state + 1;
        case 1:
            if (mag > 0x10000) {
                rec->posX = rec->posX + dst.vx / 103;
                rec->posY = rec->posY + dst.vy / 103;
            }
            break;
        case 2:
            rec->state = rec->state + 1;
        case 3:
            if (mag > 0x10000) {
                rec->posX = rec->posX + dst.vy / 103;
                rec->posY = rec->posY + dst.vx / 103;
            }
            break;
        case 4:
            rec->anim = 128;
            rec->state = rec->state + 1;
        case 5:
            tbl = taco_actor_rec;
            src.vx = tbl->posX - rec->posX;
            src.vy = (tbl->posY - rec->posY) + 128;
            src.vz = 0;
            mag = vector_normalize(&src, &dst);
            if (mag > 1600) {
                rec->posX = rec->posX + dst.vy / 103;
                rec->posY = rec->posY + dst.vx / 103;
            }
            break;
        case 6:
            rec->anim = 144;
            rec->rotX = 3 - (rand() & 7);
            rec->rotY = 3 - (rand() & 7);
            rec->state = rec->state + 1;
        case 7:
            tbl = taco_actor_rec;
            if (tbl->posZ + 33 < rec->posZ) {
                rec->velZ = -32;
            }
            if (rec->posZ < 41) {
                rec->velZ = 32;
            }
            break;
        case 8:
            rec->anim = 144;
            src.vx = 15 - (rand() & 0x1F);
            src.vy = 15 - (rand() & 0x1F);
            src.vz = 15 - (rand() & 0x1F);
            vector_normalize(&src, &dst);
            rec->rotX = dst.vx / 103;
            rec->rotY = dst.vy / 103;
            rec->velZ = dst.vz / 103;
            rec->state = rec->state + 1;
        case 9:
            if ((rand() & 0x7F) == 0) {
                rec->state = 8;
            }
            break;
        }
    } else {
        tbl = &taco_actor_rec[idx];
        src.vx = tbl->posX - rec->posX;
        src.vy = tbl->posY - rec->posY;
        src.vz = 0;
        vector_normalize(&src, &dst);
        rec->posX = rec->posX + dst.vx / 103;
        rec->posY = rec->posY + dst.vy / 103;
    }

    rec->posZ = rec->posZ + rec->velZ;
    if (rec->posX > 4096) {
        rec->posX = 4096;
    }
    if (rec->posX < -4096) {
        rec->posX = -4096;
    }
    if (rec->posY > 384) {
        rec->posY = 384;
    }
    if (rec->posY < -2048) {
        rec->posY = -2048;
    }
    if ((rand() & 0x3F) == 0) {
        rec->velZ = 15 - (rand() & 0x1F);
    }
    tbl = taco_actor_rec;
    if (tbl->posZ + 512 < rec->posZ) {
        rec->velZ = -(rand() & 0xF);
    }
    if (rec->posZ < -4096) {
        rec->velZ = rand() & 0xF;
    }
    rec->sclX = 1024;
    rec->sclY = 1024;
    rec->sclZ = 1024;
}
