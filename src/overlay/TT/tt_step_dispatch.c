/* @SymbolStart: 0x80140D74 */
/* @SymbolSize: 796 */
/* @CompileEnv: IDO52_EXCLUDE_REGS_WEB=0:v1 */
/* @CompileFlags: -Wc,-pic1 */
/* Address: 0x80140D74 (cross-prologue head absorbed), 796 bytes / 197 words */
/* (1) HEAD ABSORPTION.  `@SymbolStart: 0x80140D74` + `@SymbolSize: 796`, */
#include "types.h"

extern s32 device_cmd_send();
extern s32 change_trans_pointer();
extern s32 tt_unit_hit_step(void);
extern s32 tt_roster_init();
extern s32 tt_fall_step();
extern s32 tt_intro_seq_step();
extern s32 tt_sway_dispatch();
extern s32 tt_crowd_anim();
extern s32 tt_panel_step(void);
extern s32 tt_script_step(void);
extern s32 tt_sub_dispatch();
extern s32 tt_sprite_pool_clear(void);
extern s32 tt_shape_clear();

extern u8 *tt_step_wk;
extern u8 *tt_scroll_wk;
extern u8 *tt_stage_wk;
extern u8 *tt_player_wk;
extern u8 *tt_boss_wk;

void tt_step_dispatch(void)
{
    u8 *pMain;
    u8 *pGate;
    u8 *self;
    u8 *pRec;
    u8 *pAux;
    s32 sel;

    self = tt_boss_wk;
    sel = *(u16 *)(self + 4);
    pMain = tt_step_wk;
    pRec = tt_player_wk;

    switch (sel) {
    case 0:
        tt_sprite_pool_clear();
        if (*(u16 *)(pMain + 4) == 0) {
            device_cmd_send(177);
            change_trans_pointer(0x8019A000);
            device_cmd_send(18);
            device_cmd_send(35);
            device_cmd_send(46);
            device_cmd_send(33);
            device_cmd_send(4);
            device_cmd_send(512);
        }
        *(s16 *)(pMain + 50) = 0;
        *(s16 *)(pMain + 52) = 4096;
        tt_roster_init();
        *(s16 *)(self + 4) = 32;
        break;
    case 32:
        pGate = tt_scroll_wk;
        if (*(s32 *)pGate == 0) {
            pMain[28] = 16;
        }
        tt_fall_step();
        tt_crowd_anim();
        break;
    case 48:
        pGate = tt_scroll_wk;
        if (*(s32 *)pGate == 0) {
            pMain[28] = 16;
        }
        tt_intro_seq_step();
        tt_crowd_anim();
        break;
    case 64:
        pGate = tt_scroll_wk;
        if (*(s32 *)pGate == 0) {
            pMain[28] = 16;
        }
        tt_sway_dispatch();
        tt_crowd_anim();
        tt_sub_dispatch();
        if (*(s16 *)(self + 780) < 0) {
            switch (*(u16 *)(self + 782)) {
            case 0:
                if (*(u16 *)(self + 6) == 2) {
                    if ((u32) * (u16 *)(self + 8) < 128U) {
                        *(s16 *)(self + 8) = 128;
                    }
                } else {
                    *(s16 *)(self + 782) = 16;
                    *(s16 *)(self + 870) = 0;
                    *(s16 *)(self + 8) = 0;
                }
                break;
            case 1:
                self[807] = 0;
                *(s8 *)(self + 836) = 1;
                tt_shape_clear();
                *(s16 *)(self + 10) = 0;
                *(s16 *)(self + 8) = 0;
                *(s16 *)(self + 6) = 0;
                self[15] = 0;
                self[14] = 0;
                self[13] = 0;
                *(s16 *)(self + 4) = 80;
                break;
            case 16:
                if ((u32) * (u16 *)(self + 870) >= 60U) {
                    *(s16 *)(self + 780) = 256;
                    *(s16 *)(self + 782) = 1;
                } else {
                    *(s16 *)(self + 870) =
                        (s16)(*(u16 *)(self + 870) + 1);
                }
                break;
            }
        }
        break;
    case 80:
        tt_panel_step();
        tt_crowd_anim();
        break;
    case 128:
        device_cmd_send(193);
        *(s8 *)(pRec + 82) = 1;
        pMain[28] = 17;
        tt_script_step();
        break;
    case 160:
        device_cmd_send(177);
        self[0] = 0;
        pMain[18] = 0;
        pAux = tt_stage_wk;
        *(s16 *)(pAux + 30) = 1;
        *(s16 *)(pMain + 2) = 160;
        break;
    }

    tt_unit_hit_step();
}
