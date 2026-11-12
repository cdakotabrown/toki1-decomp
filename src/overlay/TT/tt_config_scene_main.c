/* @SymbolSize: 240 */
/* tt_config_scene_main - TT overlay, Address: 0x8013D060, 240 bytes (60 insns) */
#include "types.h"

extern u8 *tt_step_wk;

extern void tt_obj_clear(void);
extern void tt_shot_clear(void);
extern void tt_sprite_pool_reset(void);
extern void tt_pad_config(void);
extern void tt_config_page_init(void);
extern void tt_config_page_step(void);
extern void tt_slot_clear(void);
extern void tt_active_clear(void);
extern void tt_rec_clear(void);
extern void tt_ent_clear(void);
extern void trail_clear_all(void);
extern void tt_text_clear(void);
extern void tt_recs_clear(void);
extern void tt_player_shot_clear(void);
extern void tt_pool_clear(void);
extern void tt_record_clear(void);

void tt_config_scene_main(void)
{
    u8 *s0;
    u16 v0;

    s0 = tt_step_wk;
    v0 = *(u16 *)(s0 + 2);
    switch (v0) {
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
        s0[17] = 0;
        s0[28] = 0;
        s0[19] = 0;
        s0[20] = 0;
        s0[21] = 0;
        tt_config_page_init();
        *(s16 *)(s0 + 2) = 64;
        break;
    case 64:
        tt_config_page_step();
        break;
    case 80:
        tt_pad_config();
        break;
    }
}
