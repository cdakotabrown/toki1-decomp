/* @SymbolStart: 0x80136C64 */
/* @SymbolSize: 2132 */
/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
      u8  unk00;
      u8  unk01;
      u8  pad02[14];
      u16 unk10;
      s16 unk12;
      s16 unk14;
      s16 unk16;
      u8  pad18[32];
      u8  unk38;
      u8  unk39;
      u8  unk3A;
      u8  unk3B;
      s16 unk3C;
      s16 unk3E;
      s16 unk40;
      s16 unk42;
      u16 unk44;
      u8  unk46;
      u8  unk47;
      u8  pad48[28];
} TTRec;

typedef struct {
      u8  unk0;
      u8  unk1;
      u8  unk2;
      u8  pad3[3];
      u16 unk6;
      s16 unk8;
      s16 unkA;
      s16 unkC;
      s16 unkE;
      s16 unk10;
      s16 pad12;
} TTObj;

typedef struct {
      u8  pad0[8];
      u16 unk8;
      u8  padA[10];
      u16 unk14;
      u8  pad16[58];
      s16 unk50;
      u8  unk52;
} TTMain;

typedef struct {
      u16 unk0;
      u16 unk2;
      u16 unk4;
      u16 unk6;
      u8  pad8;
      u8  unk9;
      u8  unkA;
      u8  unkB;
      u8  padC[5];
      u8  unk11;
      u8  unk12;
      u8  pad13[5];
      void *unk18;
      u8  unk1C;
      u8  pad1D[5];
      u8  unk22;
} TTState;

typedef struct {
      s32 unk0;
      u8  pad4[4];
      u16 unk8;
      u8  padA[22];
} TTDelta;

typedef struct {
      union { s32 w; struct { u16 lo; u16 hi; } h; } unk0;
      u8  pad4[8];
      void (*unkC)();
      u8  pad10[16];
} TTTimer;

typedef struct {
      u8  pad0[4];
      u16 unk4;
      u16 unk6;
      u16 unk8;
      u16 unkA;
      u8  unkC;
      u8  unkD;
      u8  unkE;
      u8  unkF;
      u8  pad10[71];
      u8  unk57[9];
} TTPad;

extern void tt_mode_set();

extern TTMain  *tt_game_wk;
extern TTState *tt_step_wk;
extern TTDelta *tt_scroll_wk;
extern TTTimer *tt_stage_wk;
extern TTPad   *tt_player_wk;
extern TTPad   *tt_boss_wk;
extern TTRec   *tt_anim_buf;
extern TTObj   *tt_text_buf;
extern u8       tt_scene_seq_dat[];

void tt_scene_step()
{
    TTMain *pMain;
    TTState *pState;
    TTDelta *pDelta;
    TTTimer *pTimer;
    TTObj *pObj;
    TTRec *p;
    TTPad *pPad;
    TTPad *q;
    s32 i;
    u32 sel;
    s32 unused;
    s32 a;

    pMain = tt_game_wk;
    pDelta = tt_scroll_wk;
    pTimer = tt_stage_wk;
    pObj = tt_text_buf;
    pPad = tt_player_wk;
    pState = tt_step_wk;
    pTimer->unk0.w += pDelta->unk0;
    switch (pState->unk2) {
    case 0:
        if (pState->unk22 != 0) {
            sel = pMain->unk52;
            if (sel == 1 || sel == 2 || sel == 3 || sel == 4) {
                a = get_reads();
                if (((a - get_tt_read_prev() + 256) & 0xFF) < 5) {
                    return;
                }
                tt_load_wait();
                device_cmd_send(33);
                device_cmd_send(34);
                device_cmd_send(47);
                device_cmd_send(4);
                device_cmd_send(513);
            } else {
                device_cmd_send(177);
            }
        } else {
            device_cmd_send(33);
            device_cmd_send(34);
            device_cmd_send(47);
            device_cmd_send(4);
            device_cmd_send(513);
        }
        bzero(pDelta, 32);
        bzero(pTimer, 32);
        pMain->unk50 = 0;
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
        if (pState->unk22 != 0) {
            pState->unk11 = 0;
        } else {
            pState->unk11 = 1;
        }
        tt_trail_pool_init();
        tt_player_shot_init();
        tt_recs_init();
        tt_scene_init();
        tt_pool_reset();
        tt_obj_reset();
        pState->unk12 = 1;
        pState->unk18 = tt_scene_seq_dat;
        pState->unk1C = 2;
        tt_replay_set();
        tt_mode_set();
        pObj->unkA = -112;
        pObj->unkC = 16;
        pObj->unkE = 13;
        pObj->unk2 = 1;
        pObj->unk0 = 1;
        pObj->unk6 = 0;
        pObj->unk10 = 0;
        pObj++;
        pObj->unkA = -72;
        pObj->unkC = 16;
        pObj->unkE = 39;
        pObj->unk2 = 1;
        pObj->unk0 = 1;
        pObj->unk6 = 0;
        pObj->unk10 = 0;
        tt_time_set(pDelta->unk8, 39);
        p = tt_anim_buf;
        for (i = 0; i < 2; i++, p++) {
            p->unk00 = 1;
            p->unk01 = 0;
            p->unk44 = 0;
            p->unk14 = i * 16 - 112;
            p->unk16 = 24;
            p->unk3C = 16;
            p->unk3E = 16;
            p->unk46 = 0;
            p->unk47 = 112;
            p->unk40 = 10;
            p->unk42 = 15371;
            p->unk3B = 101;
            p->unk10 = 0;
        }
        tt_sprite_clear();
        for (i = 0; i < 6; i++, p++) {
            p->unk00 = 1;
            p->unk01 = 0;
            p->unk44 = 0;
            p->unk14 = -120;
            p->unk16 = 216 - i * 8;
            p->unk3E = 8;
            p->unk3C = 16;
            p->unk46 = (i / 2) * 16 + 80;
            p->unk47 = (i & 1) * 8 + 48;
            p->unk40 = 10;
            p->unk42 = 15369;
            p->unk3B = 101;
            p->unk10 = 0;
        }
        pPad->unk57[1] = pPad->unk57[2] = 0;
        pState->unk2 = 64;
        break;
    case 1:
        tt_load_wait2();
        device_cmd_send(33);
        device_cmd_send(34);
        device_cmd_send(47);
        device_cmd_send(4);
        device_cmd_send(513);
        tt_player_shot_clear();
        tt_rec_clear();
        tt_recs_clear();
        tt_pool_clear();
        tt_obj_clear();
        tt_shot_clear();
        tt_sprite_pool_reset();
        tt_ent_clear();
        tt_slot_clear();
        pState->unk11 = 1;
        pPad->unk57[0] = 0;
        pPad->unk57[1] = pPad->unk57[2] = 0;
        pPad->unk57[3] = 0;
        pPad->unk57[4] = 0;
        pPad->unk57[5] = 0;
        pPad->unk57[6] = 0;
        pPad->unk57[7] = 0;
        pPad->unk57[8] = 0;
        pPad->unk4 = 32;
        pPad->unkD = pPad->unkE = pPad->unkF = 0;
        pPad->unk6 = pPad->unk8 = pPad->unkA = 0;
        tt_player_shot_init();
        pState->unk12 = 1;
        pState->unk1C = 2;
        tt_mode_set();
        pState->unk2 = 64;
        break;
    case 64:
        tt_time_set(pDelta->unk8, 39);
        if (pTimer->unk0.h.hi > 1024) {
            q = tt_boss_wk;
            q->unk4 = 0;
            q->unkC = 0;
            q->unk6 = q->unk8 = q->unkA = 0;
            q->unkD = q->unkE = q->unkF = 0;
            pState->unk2 = 80;
        }
        break;
    case 80:
        tt_time_set(pDelta->unk8, 39);
        pTimer->unkC();
        break;
    case 160:
        tt_time_set(pDelta->unk8, 39);
        pPad->unk4 = 160;
        break;
    }
    if (pState->unk22 != 0 && pMain->unk52 == 255) {
        pObj = tt_text_buf + 126;
        if ((pMain->unk8 >> 4) & 1) {
            pObj->unk6 = 1;
        } else {
            pObj->unk6 = 0;
        }
        if ((pMain->unk14 & 0x800) || (pMain->unk14 & 0x20)) {
            pState->unk0 = 1;
            pState->unk2 = pState->unk4 = pState->unk6 = 0;
            pState->unk9 = pState->unkA = pState->unkB = 0;
        }
    }
    trail_step_all();
    tt_player();
    tt_sprite_step();
    tt_rec_step();
    tt_recs_step();
    tt_sprite_pool_step();
    tt_ent_step_all();
    tt_slot_step();
    tt_actor_pool_step();
    obj_pool_step();
    tt_scroll_speed_step();
    tt_speed_mark_set();
    tt_scene_hit_step();
}
