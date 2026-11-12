/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u8  pad00;
    u8  f01;
    u8  pad02[114];
    s16 f116;
    s16 f118;
    s16 f120;
    s16 f122;
    u8  pad7C[8];
    u8  f132;
    u8  f133;
    u8  f134;
    u8  f135;
} Entry;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    u8  pad06[12];
    u8  f18;
    u8  f19;
} Obj;

typedef struct {
    u8 b[9];
} Blk9;

extern Entry *taco_actor_rec;
extern s32 taco_score;
extern s32 taco_hit_flag;
extern u32 taco_hit_score_tbl[11];
extern Blk9 taco_obj_hit_tbl;
extern s32 frame_counter;

extern void taco_spark_spawn(s16, s16, s16);

taco_obj_hit(objs, arg1, arg2)
Obj *objs;
s32 arg1;
s32 arg2;
{
    s32 dx, dy;
    s32 i;
    s32 j;
    s32 x, y, z;
    s32 r;
    s32 w;
    s32 dmg;
    s16 hx, hy;
    Blk9 buf;

    buf = taco_obj_hit_tbl;
    taco_hit_flag = 0;
    if (objs[0].f19 & 0x20) {
        for (i = 0; i < 2; i++) {
            if (frame_counter & 1) {
                x = (i + objs)->x;
                y = (i + objs)->y;
                if (arg2 == 0) {
                    z = taco_actor_rec[0].f120;
                } else {
                    z = taco_actor_rec[8].f120;
                }
                for (j = 16; j < 63; j++) {
                    if (((j + taco_actor_rec)->f01 & 0x80) && ((j + taco_actor_rec)->f135 & 1) && (j + taco_actor_rec)->f133 && (j + taco_actor_rec)->f132) {
                        r = (j + taco_actor_rec)->f132 * 16;
                        dx = abs(x - (j + taco_actor_rec)->f116);
                        dy = abs(y - (j + taco_actor_rec)->f118);
                        if (dx < r + 24 && dy < r + 24 && (j + taco_actor_rec)->f120 < z) {
                            hx = x;
                            hy = y;
                            if (taco_hit_flag == 0 && arg2 == 0) {
                                taco_hit_flag = 1;
                                sd_cmd_limit_send(1, 1287);
                            }
                            if ((j + taco_actor_rec)->f133) {
                                (j + taco_actor_rec)->f133--;
                            }
                            taco_spark_spawn(hx, hy, (j + taco_actor_rec)->f120 - 512);
                            taco_score += taco_hit_score_tbl[(taco_actor_rec + j)->f134 % 11] / 20;
                            if ((taco_actor_rec + j)->f133 == 0) {
                                taco_score += taco_hit_score_tbl[(taco_actor_rec + j)->f134 % 11];
                                if (taco_score >= 10000000) {
                                    taco_score = 9999999;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    for (i = 0; i < 16; i++) {
        if ((i + objs)->f19 & 0x80) {
            if ((i + objs)->f19 & 0xF) {
                x = (i + objs)->x;
                y = (i + objs)->y;
                z = (i + objs)->z;
                dmg = buf.b[(i + objs)->f19 & 0xF];
                if (((i + objs)->f19 & 0xF) == 8) {
                    w = 96;
                } else {
                    w = 48;
                }
                for (j = 16; j < 63; j++) {
                    if (((j + taco_actor_rec)->f01 & 0x80) && ((j + taco_actor_rec)->f135 & 1) && (j + taco_actor_rec)->f133 && (j + taco_actor_rec)->f132) {
                        r = (j + taco_actor_rec)->f132 * 16;
                        if (abs(z - (j + taco_actor_rec)->f120) < r + 256) {
                            dx = abs(x - (j + taco_actor_rec)->f116);
                            dy = abs(y - (j + taco_actor_rec)->f118);
                            if (dx < r + w && dy < r + w) {
                                if (taco_hit_flag == 0 && arg2 == 0) {
                                    taco_hit_flag = 1;
                                    sd_cmd_limit_send(1, 1287);
                                }
                                if (dmg < (j + taco_actor_rec)->f133) {
                                    (j + taco_actor_rec)->f133 -= dmg;
                                } else {
                                    (j + taco_actor_rec)->f133 = 0;
                                }
                                if ((j + taco_actor_rec)->f133 == 0) {
                                    (j + taco_actor_rec)->f135 &= 0xF0;
                                }
                                (i + objs)->f18 = 0;
                                (i + objs)->f19 = 0;
                                taco_spark_spawn(((j + taco_actor_rec)->f116 + x) / 2, ((j + taco_actor_rec)->f118 + y) / 2, z - 512);
                                taco_score += taco_hit_score_tbl[(taco_actor_rec + j)->f134 % 11] / 10 * dmg;
                                if ((taco_actor_rec + j)->f133 == 0) {
                                    sd_cmd_limit_send(1, 1288);
                                    taco_score += taco_hit_score_tbl[(taco_actor_rec + j)->f134 % 11];
                                    if (taco_score >= 10000000) {
                                        taco_score = 9999999;
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}
