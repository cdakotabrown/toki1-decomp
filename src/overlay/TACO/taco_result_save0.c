/* @SymbolStart: 0x801427AC */
/* @SymbolSize: 532 */
/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u8 b0 : 1;
    u8 b1 : 1;
    u8 b2 : 1;
    u8 b3 : 2;
    u8 b5 : 3;
    u8 lo : 4;
    u8 hi : 4;
    u8 name[3][3];
    s32 score[3];
} Rec;

extern Rec taco_rec;
extern s32 taco_rank_score[3];
extern u8 taco_rank_name[];
extern u8 taco_rank_name_1_0[];
extern u8 taco_rank_name_2_0[];
extern u8 taco_rensya_flag;
extern u8 taco_turn_btn;
extern u8 taco_shot_btn;
extern u8 taco_burst_btn;
extern s16 disp_ofs_x[];

void taco_result_save0()
{
    taco_rec.b0 = 1;
    taco_rec.score[0] = taco_rank_score[0];
    taco_rec.score[1] = taco_rank_score[1];
    taco_rec.score[2] = taco_rank_score[2];
    strncpy(taco_rec.name[0], taco_rank_name, 3);
    strncpy(taco_rec.name[1], taco_rank_name_1_0, 3);
    strncpy(taco_rec.name[2], taco_rank_name_2_0, 3);
    taco_rec.b2 = taco_rensya_flag & 1;
    taco_rec.b3 = taco_turn_btn % 3;
    taco_rec.hi = taco_shot_btn % 3;
    taco_rec.lo = taco_burst_btn % 3;
    prim_init();
    draw2d3d(0, 0);
    disp_ofs_x[0] = 160;
    disp_ofs_x[1] = 160;
    DrawSync(0);
    ResetGraph(3);
    GsInitGraph(320, 240, 0, 0, 0);
    if (cdq_idle_check() == 0) {
        CDQ_init();
    }
    gs_env_init();
    all_object_disp_off();
    taco_obj_clear();
    class_clear_all(0);
    class_switch(0);
    sub_sub_step_up();
}
