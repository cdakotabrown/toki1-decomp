/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=0x80162000 */
/* @PostProc: swap_instructions --offset1 0xEC --offset2 0xF0 */
/* @PostProc: swap_instructions --offset1 0xF0 --offset2 0xF4 */
/* Address: 0x80148154   Size: 388 bytes (97 instructions) */
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
extern StructA taco_maina_key_tbl[];
extern StructB taco_maina_mode_tbl[];
extern StructC taco_obj_1_attribute[];
extern s16 taco_angle_tbl[][4];

extern void taco_tile_load();
extern void taco_maina_light_set();
extern void pose_tbl_reset();
extern void taco_obj_link2();
extern void image_upload_queue_add_flags();
extern void taco_rec_move();
extern void node_pose_set();
extern void taco_stage_reset();
extern void sub_sub_step_up();

void
taco_maina_init0(void)
{
    s16 *dp;
    StructC *pc;
    s32 i;

    if (taco_load_no != 0) {
        return;
    }
    taco_load_no = 32;
    taco_tile_load((void *)0x80162000, 14);
    taco_tile_load((void *)0x80172000, 12);
    taco_maina_light_set();
    pose_tbl_reset(0);
    taco_obj_link2((void *)0x8018D000);
    image_upload_queue_add_flags((void *)0x80162000, 8, 1, 0, 0, 256, 256);

    dp = taco_actor_rec;
    taco_rec_move(0, dp[58] - 3000, dp[59] + 1000, 0xFFFEF660, 0, 190, 0, 0,
                  0);

    pc = taco_obj_1_attribute;
    for (i = 1; i != 31; i++) {
        node_pose_set(i, 0, taco_maina_key_tbl[i - 1].a, taco_maina_key_tbl[i - 1].b,
                      taco_maina_mode_tbl[i - 1].b0, taco_maina_mode_tbl[i - 1].b1);
        pc++;
        pc[-1].w[0] = 0;
    }

    taco_angle_tbl[0][0] = 0;
    taco_angle_tbl[0][1] = 190;
    taco_angle_tbl[0][2] = 0;

    taco_stage_reset(0);
    sub_sub_step_up();
}
