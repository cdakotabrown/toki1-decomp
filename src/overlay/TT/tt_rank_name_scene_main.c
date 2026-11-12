/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=60 IDO52_FORCE_WEB_COLOR=123:a0,54:v1 IDO52_UGEN_TEMP_REG=20:t7,21:t6,22:t8,23:t9,24:t0,25:t1 */
/* @SymbolSize: 696 */
/* tt_rank_name_scene_main - TT overlay, Address: 0x80146914, Size: 696 bytes (174 insns) */
#include "types.h"

extern u8 *tt_step_wk;
extern u8 *tt_scroll_wk;
extern u8 *tt_anim_buf;
extern u8 *tt_text_buf;

extern void device_cmd_send(s32 id);
extern void tt_record_clear(void);
extern void tt_text_clear(void);
extern void tt_active_clear(void);
extern void tt_player_shot_clear(void);
extern void tt_rec_clear(void);
extern void tt_recs_clear(void);
extern void trail_clear_all(void);
extern void tt_pool_clear(void);
extern void tt_obj_clear(void);
extern void tt_shot_clear(void);
extern void tt_sprite_pool_reset(void);
extern void tt_ent_clear(void);
extern void tt_slot_clear(void);
extern s32 tt_rank_insert(void);
extern void tt_layout_init(void);
extern void tt_row_init(s32 a0, s32 a1, s32 a2);
extern void tt_board_init(s32 a0, s32 a1);
extern void tt_fade_in20(void);
extern void tt_board_scale_step(void);
extern void tt_name_key40(void);
extern void tt_fade_out80(void);
extern void tt_secs_set(u32 a0, s32 a1);

void tt_rank_name_scene_main(void)
{
    s32 unused;
    u8 *s0;
    u8 *blk;
    u8 *rec;
    volatile u8 tmp;
    u8 v;
    s32 state;
    s32 c128;

    s0 = tt_step_wk;
    blk = tt_scroll_wk;
    state = *(u16 *)(s0 + 2);
    c128 = 128;

    switch (state) {
    case 0:
        device_cmd_send(34);
        device_cmd_send(47);
        device_cmd_send(4);
        device_cmd_send(518);
        s0[28] = 1;
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
        if (tt_rank_insert() != 0) {
            s0[29] = 0;
            s0[30] = 0;
            s0[31] = 0;
            tt_layout_init();
            tt_row_init(2, 6, 4);
            tt_board_init(12, 1);
            *(s16 *)(blk + 30) = 0;
        } else {
            s0[9] = 1;
        }
        *(s16 *)(s0 + 38) = 3600;
        *(s16 *)(s0 + 2) = 32;
        break;

    case 32:
        tt_fade_in20();
        break;

    case 64:
        tt_board_scale_step();
        tt_name_key40();
        if (s0[10] == 0) {
            if (--*(u16 *)(s0 + 38) != 0) {
                break;
            }
        }
        rec = tt_text_buf + *(u16 *)(blk + 24) * 60;
        tt_anim_buf[0] = 0;
        s0[10] = 0;
        *(s16 *)(s0 + 4) = 0;
        *(s16 *)(s0 + 2) = (s16)c128;
        *(s16 *)(s0 + 40) = 60;
        switch (*(u16 *)(blk + 24)) {
        case 0:
            tmp = 2;
            break;
        case 1:
            tmp = 6;
            break;
        case 2:
            tmp = 4;
            break;
        }
        v = tmp;
        rec[42] = v;
        rec[22] = v;
        rec[2] = v;
        *(s16 *)(s0 + 2) = 112;
        break;

    case 112:
        tt_board_scale_step();
        if (--*(u16 *)(s0 + 40) != 0) {
            break;
        }
        s0[29] = (u8)c128;
        s0[30] = (u8)c128;
        s0[31] = (u8)c128;
        *(s16 *)(s0 + 2) = (s16)c128;
        break;

    case 128:
        tt_fade_out80();
        break;
    }

    if (s0[9] != 0) {
        *(s16 *)(s0 + 0) = 7;
        *(s16 *)(s0 + 6) = 0;
        *(s16 *)(s0 + 4) = 0;
        *(s16 *)(s0 + 2) = 0;
        s0[11] = 0;
        s0[10] = 0;
        s0[9] = 0;
    }
    tt_secs_set((u32)*(u16 *)(s0 + 38) / 60U, 38);
}
