#include "types.h"

extern s32 bzero();
extern s32 *tt_step_wk;
extern s32 *tt_disp_wk;
extern s32 *tt_scroll_wk;
extern s32 *tt_stage_wk;
extern s32 *tt_player_wk;
extern s32 *tt_trail_buf;
extern s32 *tt_cel_buf;
extern s32 *tt_player_shot_buf;
extern s32 *tt_rec_pool;
extern s32 *tt_recs_buf;
extern s32 *tt_boss_wk;
extern s32 *tt_bell_buf;
extern s32 *tt_ent_buf;
extern s32 *tt_slot_buf;
extern s32 *tt_obj_pool;
extern s32 *tt_shot_buf;
extern s32 *tt_anim_buf;
extern s32 *tt_text_buf;

void tt_buffers_clear(void)
{
    bzero(tt_step_wk, 56);
    tt_disp_wk[2] = 0;
    bzero(tt_text_buf, 2560);
    bzero(tt_scroll_wk, 32);
    bzero(tt_stage_wk, 32);
    bzero(tt_player_wk, 116);
    bzero(tt_trail_buf, 492);
    bzero(tt_cel_buf, 768);
    bzero(tt_player_shot_buf, 1008);
    bzero(tt_rec_pool, 704);
    bzero(tt_recs_buf, 704);
    bzero(tt_boss_wk, 1060);
    bzero(tt_bell_buf, 3456);
    bzero(tt_ent_buf, 624);
    bzero(tt_slot_buf, 7680);
    bzero(tt_obj_pool, 3840);
    bzero(tt_shot_buf, 13312);
    bzero(tt_anim_buf, 12800);
}
