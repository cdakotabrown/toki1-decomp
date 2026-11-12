/* @PostProc: scheduler_normalizer */
/* Address: 0x8014A040 */
#include "types.h"

typedef struct {
    s32 a;
    s32 b;
} StructA;

typedef struct {
    u8 b0;
    u8 b1;
} StructB;

typedef struct {
    s32 w[4];
} StructC;

extern u8 taco_load_no;
extern s16 *taco_actor_rec;
extern StructA taco_main16_key_tbl[];
extern StructB taco_main16_mode_tbl[];
extern StructC taco_obj_1_attribute[];
extern s16 taco_angle_tbl[][4];

extern void taco_main16_light_set();
extern void pose_tbl_reset();
extern void taco_obj_link2();
extern void rec_word_copy();
extern void taco_tile_load();
extern void taco_rec_move();
extern void node_pose_set();
extern void taco_object_slot_init();
extern void sub_sub_step_up();

void
taco_main16_init0(void)
{
    StructA *pa;
    StructB *pb;
    StructC *pc;
    s16 *dp;
    s32 i;

    if (taco_load_no != 0) {
        return;
    }
    taco_main16_light_set();
    pose_tbl_reset(0);
    taco_obj_link2((void *)0x801A7000);
    rec_word_copy((void *)0x801A7000);
    taco_tile_load((void *)0x80162000, 8);
    taco_tile_load((void *)0x80172000, 10);

    dp = taco_actor_rec;
    taco_rec_move(0, dp[58], dp[59] - 3000, -15000, 30, 180, 0, 0, 0);

    pa = &taco_main16_key_tbl[1];
    pb = &taco_main16_mode_tbl[1];
    pc = taco_obj_1_attribute;
    for (i = 1; i < 34; i++) {
        node_pose_set(i, 0, pa[-1].a, pa[-1].b, pb[-1].b0, pb[-1].b1);
        pa++;
        pb++;
        pc++;
        pc[-1].w[0] = 0;
    }

    taco_angle_tbl[0][0] = 30;
    taco_angle_tbl[0][1] = 180;
    taco_angle_tbl[0][2] = 0;

    for (i = 16; i != 63; i++) {
        taco_object_slot_init(i);
    }

    sub_sub_step_up();
}
