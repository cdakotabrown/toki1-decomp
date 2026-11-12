/* @SymbolSize: 1260 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_EXCLUDE_REGS_WEB=34:s8 IDO52_FORCE_WEB_NOSPILL=34.1/1 IDO52_WEB_PIECE_NOREAPPLY=34.1 IDO52_UGEN_TEMP_REG=23:t0,24:t1,25:t2,26:t3,27:t4,28:t9,29:t5,30:t5,31:t7,39:t5,40:t9 */
#include "types.h"

typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad;
} Vec8;

typedef struct {
    u8  f00;
    u8  f01;
    u8  f02;
    u8  f03;
    s32 f04;
    s32 f08;
    s32 pad0C[2];
    s32 f20[20];
    s16 f100;
    s16 f102;
    s16 f104;
    s16 f106;
    s16 f108;
    s16 f110;
    s16 f112;
    s16 f114;
    s16 f116;
    s16 f118;
    s16 f120;
    s16 f122;
    s16 f124;
    s16 f126;
    s16 f128;
    s16 f130;
    u8  f132;
    u8  f133;
    u8  f134;
    u8  f135;
} Entry;

typedef struct {
    u32 a;
    u32 b;
} Pair;

typedef struct {
    s32 f0;
    s32 f4;
    s32 f8;
    s32 fC;
} Node16;

typedef struct {
    s32 w[20];
} Rec80;

extern Entry *taco_actor_rec;

extern u8 taco_node_own[];
extern s32 *taco_kind_mdl_tbl[];
extern u8 taco_kind_node_tbl[];
extern u32 *taco_kind_pose_tbl[];
extern s16 taco_kind_scl_tbl[];
extern u8 taco_kind_rad_tbl[];

extern Node16 taco_obj[];
extern Rec80 taco_coord[];

void GsInitCoordinate2();
void GsLinkObject4();
void GsMapModelingData();
void taco_object_slot_init();
void taco_rec_matrix_set();
void taco_rec_pose_set();

s32 taco_actor_alloc(kind, arg1, vA, vB, vC, arg5, arg6)
    s32 kind;
    s32 arg1;
    Vec8 vA;
    Vec8 vB;
    Vec8 vC;
    s32 arg5;
    s32 arg6;
{
    Pair *e;
    s32 *base;
    s32 slot;
    s32 i;
    s32 n;

    slot = 0;
    n = 0;
    for (i = 16; i < 63; i++) {
        if (!(taco_actor_rec[i].f01 & 0x80)) {
            slot = i;
            break;
        }
    }
    if (slot == 0) {
        return -1;
    }
    for (i = 0; i < 64; i++) {
        if (taco_node_own[i] == 0) {
            n++;
        }
    }
    if (n < taco_kind_node_tbl[kind]) {
        return -1;
    }
    base = taco_kind_mdl_tbl[kind] + 1;
    GsMapModelingData(base);
    base += 2;
    taco_object_slot_init(slot);
    GsInitCoordinate2(0, (slot + taco_actor_rec)->f20);
    if (kind == 10) {
        GsLinkObject4(base, &taco_actor_rec[slot].f04, 1);
    } else {
        GsLinkObject4(base, &taco_actor_rec[slot].f04, 0);
    }
    n = taco_kind_node_tbl[kind];
    i = 0;
    if (n != 0) {
        do {
            if (taco_node_own[i] == 0) {
                GsInitCoordinate2((slot + taco_actor_rec)->f20, &taco_coord[i]);
                taco_node_own[i] = slot;
                if (kind == 15 || kind == 16) {
                    e = (taco_kind_node_tbl[kind] - n) + (Pair *)taco_kind_pose_tbl[kind];
                    taco_rec_matrix_set(i, e->a, e->b, 0);
                } else {
                    taco_rec_pose_set(i, taco_kind_pose_tbl[kind][taco_kind_node_tbl[kind] - n]);
                }
                GsLinkObject4(base, &taco_obj[i], taco_kind_node_tbl[kind] - n);
                n--;
                taco_obj[i].f4 = (s32)&taco_coord[i];
                taco_obj[i].f0 = 0x80000000;
            }
            i++;
        } while (n != 0);
    }
    taco_actor_rec[slot].f04 = 0x80000000;
    taco_actor_rec[slot].f02 = arg1;
    taco_actor_rec[slot].f122 = 32767;
    taco_actor_rec[slot].f134 = kind;
    taco_actor_rec[slot].f03 = 1;
    taco_actor_rec[slot].f130 = 0;
    taco_actor_rec[slot].f114 = 0;
    taco_actor_rec[slot].f124 = vA.vx;
    taco_actor_rec[slot].f126 = vA.vy;
    taco_actor_rec[slot].f128 = vA.vz;
    taco_actor_rec[slot].f01 = 128;
    taco_actor_rec[slot].f116 = vB.vx;
    taco_actor_rec[slot].f118 = vB.vy;
    taco_actor_rec[slot].f120 = vB.vz;
    taco_actor_rec[slot].f100 = vC.vx;
    taco_actor_rec[slot].f102 = vC.vy;
    taco_actor_rec[slot].f104 = vC.vz;
    taco_actor_rec[slot].f106 = vC.pad;
    taco_actor_rec[slot].f108 = taco_kind_scl_tbl[kind];
    taco_actor_rec[slot].f110 = taco_kind_scl_tbl[kind];
    taco_actor_rec[slot].f112 = taco_kind_scl_tbl[kind];
    taco_actor_rec[slot].f133 = arg5;
    taco_actor_rec[slot].f132 = taco_kind_rad_tbl[kind];
    taco_actor_rec[slot].f135 = arg6 | 1;
    (slot + taco_actor_rec)->f08 = (s32)(slot + taco_actor_rec)->f20;
    return slot;
}
