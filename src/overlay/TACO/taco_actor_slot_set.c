/* @SymbolSize: 508 */
/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u8 pad00;
    u8 f01;
    u8 pad02[2];
    s32 f04;
    s32 f08;
    u8 pad0C[88];
    s16 f64;
    s16 f66;
    s16 f68;
    u8 pad6A[10];
    s16 f74;
    s16 f76;
    s16 f78;
    s16 f7A;
    u8 pad7C[6];
    s16 f82;
    u8 f84;
    u8 f85;
    u8 pad86;
    u8 f87;
} Rec136;

extern Rec136 *taco_actor_rec;
extern s32 taco_coord[][20];
extern s16 taco_angle_tbl[][4];

taco_actor_slot_set(k, i, p, tbl, pairs, stride)
s32 k;
s32 i;
s32 *p;
u8 *tbl;
u8 *pairs;
s32 stride;
{
    s32 *base;
    s32 x;
    s32 n;

    n = i + 16;
    taco_object_slot_init(n);
    base = p + 1;
    GsMapModelingData(base);
    base += 2;
    GsLinkObject4(base, &taco_actor_rec[n].f04, k - 1);
    taco_actor_rec[n].f04 = 0x80000000;
    taco_actor_rec[n].f01 = 128;
    taco_actor_rec[n].f74 = taco_coord[k][6];
    taco_actor_rec[n].f76 = taco_coord[k][7];
    taco_actor_rec[n].f78 = taco_coord[k][8];
    taco_actor_rec[n].f64 = taco_angle_tbl[k][0];
    taco_actor_rec[n].f66 = taco_angle_tbl[k][1];
    taco_actor_rec[n].f68 = taco_angle_tbl[k][2];
    taco_actor_rec[n].f85 = pairs[i * 2];
    taco_actor_rec[n].f7A = pairs[i * 2 + 1];
    taco_actor_rec[n].f82 = 1;
    taco_actor_rec[n].f84 = tbl[i * stride];
    taco_actor_rec[n].f87 = 1;
    taco_actor_rec[n].f08 = taco_coord[k];
}
