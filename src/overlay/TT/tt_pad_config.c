/* @SymbolStart: 0x8013CB58 */
/* @SymbolSize: 204 */
/* tt_pad_config - TT overlay, Address: 0x8013CB60, Size: 196 bytes (49 insns) */
#include "types.h"

extern void *tt_step_wk;
extern s32 tt_obj_clear();
extern s32 tt_shot_clear();
extern s32 tt_sprite_pool_reset();
extern s32 tt_pad_config_init(void);
extern s32 tt_pad_config_cursor();
extern s32 tt_slot_clear();
extern s32 tt_active_clear();
extern s32 tt_rec_clear();
extern s32 tt_ent_clear();
extern s32 trail_clear_all();
extern s32 tt_text_clear();
extern s32 tt_recs_clear();
extern s32 tt_player_shot_clear();
extern s32 tt_pool_clear();
extern s32 tt_record_clear();

void tt_pad_config(void)
{
    void *t6_hidden = tt_step_wk;

    switch (*(u16 *)((u8 *)t6_hidden + 4)) {
    case 0:
        tt_record_clear();
        tt_text_clear();
        tt_active_clear();
        tt_player_shot_clear();
        tt_rec_clear();
        tt_recs_clear();
        trail_clear_all();
        tt_pool_clear();
        tt_obj_clear();
        tt_shot_clear();
        tt_sprite_pool_reset();
        tt_ent_clear();
        tt_slot_clear();
        tt_pad_config_init();
        *(s16 *)((u8 *)t6_hidden + 4) = 64;
        break;
    case 64:
        tt_pad_config_cursor();
        break;
    }
}
