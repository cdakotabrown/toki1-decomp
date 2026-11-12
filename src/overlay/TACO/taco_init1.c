/* @SymbolStart: 0x8014483C */
/* @SymbolSize: 268 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=8 */
/* Address: 0x80144850 (true entry 0x8014483C, 5 insns hoisted above the .sym: */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern void sub_sub_step_up(void);
extern void draw2d3d(s32, s32);
extern void initView(void);
extern void DrawSync(s32);
extern void taco_star_init(void *);
extern void taco_clut_move(s32);
extern void pose_tbl_reset(s32);
extern void func_80146DEC(void);
extern void taco_obj_link2(s32);
extern void taco_pose_keyframe_set(s32, s32, s32);

extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;
extern u8 cursor_work_bak;
extern u8 ovl_work_buf;
extern s32 taco_cut_key_tbl;
extern s32 taco_cut_key_tbl_1;
extern s16 taco_angle_tbl[][4];
extern LIBGS_DOBJ2 taco_obj[];

void taco_init1(void)
{
    back_clear_r = 8;
    back_clear_b = 16;
    back_clear_g = 8;
    draw2d3d(1, 1);
    initView();
    func_80146DEC();
    pose_tbl_reset(-4000);
    taco_obj_link2(0x8019C800);
    taco_star_init(&cursor_work_bak);
    taco_star_init(&ovl_work_buf);
    DrawSync(0);
    taco_clut_move(7);
    taco_pose_keyframe_set(1, taco_cut_key_tbl, 0);
    (*(s32 *)&taco_obj[1].attribute) = 0;
    taco_angle_tbl[1][0] = -90;
    taco_angle_tbl[1][2] = 0;
    taco_angle_tbl[1][1] = 90;
    taco_pose_keyframe_set(2, taco_cut_key_tbl_1, 0);
    (*(s32 *)&taco_obj[2].attribute) = 0;
    taco_angle_tbl[2][0] = -90;
    taco_angle_tbl[2][2] = 0;
    taco_angle_tbl[2][1] = -90;
    sub_sub_step_up();
}
