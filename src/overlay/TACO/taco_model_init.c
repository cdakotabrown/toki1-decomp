/* @SymbolStart: 0x80134940 */
/* @SymbolSize: 232 */
/* Address: 0x8013494C */
/* Size: 232 bytes (58 insns incl. the hoisted cross-prologue head at 0x80134940; */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;
extern s32 taco_model_pose_tbl[];
extern s16 taco_light_cnt;
extern s16 taco_angle_tbl[][4];
extern LIBGS_DOBJ2 taco_obj[];

extern void draw2d3d(s32 a, s32 b);
extern void initView(void);
extern void initModelingData_init(void *p);
extern void pose_tbl_reset(s32 a);
extern void taco_obj_link2(void *p);
extern void taco_pose_keyframe_set(s32 a, s32 b, s32 c);

void taco_model_init(void)
{
    back_clear_g = 0;
    back_clear_r = 0;
    back_clear_b = 0;
    draw2d3d(1, 1);
    initView();
    pose_tbl_reset(-4000);
    initModelingData_init((void *)0x8019C800);
    taco_obj_link2((void *)0x8019C800);
    taco_pose_keyframe_set(1, taco_model_pose_tbl[0], 0);
    (*(s32 *)&taco_obj[1].attribute) = 0;
    taco_angle_tbl[1][0] = 50;
    taco_angle_tbl[1][2] = 50;
    taco_angle_tbl[1][1] = 210;
    taco_pose_keyframe_set(2, taco_model_pose_tbl[1], 0);
    (*(s32 *)&taco_obj[2].attribute) = 0;
    taco_angle_tbl[2][0] = 75;
    taco_angle_tbl[2][2] = -50;
    taco_angle_tbl[2][1] = 195;
    taco_light_cnt = 0;
}
