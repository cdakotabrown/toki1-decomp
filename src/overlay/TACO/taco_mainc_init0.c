/* @PostProc: scheduler_normalizer */
/* Address: 0x80149084 */
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
extern StructA taco_mainc_key_tbl[];
extern StructB taco_mainc_mode_tbl[];
extern StructC taco_obj_1_attribute[];
extern s16 taco_angle_tbl[][4];
extern s32 taco_mainc_ang_tbl[];

extern void taco_tile_load();
extern void func_80146DEC();
extern void pose_tbl_reset();
extern void taco_obj_link2();
extern void rec_word_copy();
extern void taco_rec_move();
extern void node_pose_set();
extern void taco_rows_disp();
extern void angle_set();
extern void taco_mainc_reset();
extern void sub_sub_step_up();

void
taco_mainc_init0(void)
{
    StructA *pa;
    StructB *pb;
    StructC *pc;
    s16 *dp;
    s32 *pe;
    s32 i;

    if (taco_load_no != 0) {
        return;
    }
    taco_load_no = 48;
    taco_tile_load((void *)0x80172000, 14);
    taco_tile_load((void *)0x80162000, 12);
    func_80146DEC();
    pose_tbl_reset(0);
    taco_obj_link2((void *)0x80196000);
    rec_word_copy((void *)0x80196000);
    taco_tile_load((void *)0x80162000, 8);

    dp = taco_actor_rec;
    taco_rec_move(0, dp[58], dp[59] + 1100, -1600, -10, 180, 0, 0, 0);

    pa = &taco_mainc_key_tbl[1];
    pb = &taco_mainc_mode_tbl[1];
    pc = taco_obj_1_attribute;
    for (i = 1; i < 36; i++) {
        node_pose_set(i, 0, pa[-1].a, pa[-1].b, pb[-1].b0, pb[-1].b1);
        pa++;
        pb++;
        pc++;
        pc[-1].w[0] = 0;
    }

    taco_angle_tbl[0][0] = -10;
    taco_angle_tbl[0][1] = 180;
    taco_angle_tbl[0][2] = 0;

    taco_rows_disp(1, 0);
    taco_rows_disp(2, 7);
    taco_rows_disp(3, 14);

    pe = &taco_mainc_ang_tbl[1];
    for (i = 29; i != 33; i++) {
        angle_set(i, *pe);
        pe++;
    }
    angle_set(35, taco_mainc_ang_tbl[0]);

    taco_mainc_reset();
    sub_sub_step_up();
}
