/* @SymbolSize: 996 */
/* @PostProc: swap_instructions --offset1 0x94 --offset2 0x98 */
/* @PostProc: swap_instructions --offset1 0x98 --offset2 0x9c */
#include "types.h"
#include "tokimeki/save_work.h"

extern u16 tt_config;
extern u8 D_800F022B;
extern u8 D_800F022C;
extern u8 tt_config_flags_b;
extern u8 tt_config_flags_u;
extern u8 tt_result;
extern u8 *tt_game_wk;
extern u8 *tt_step_wk;
extern u8 *tt_disp_wk;
extern u8 *tt_scroll_wk;
extern void (*tt_main_fn_tbl[])(void);
extern u8 tt_twinbee_start_fmt[];
extern u8 tt_open_event_fmt[];
extern u8 tt_main_str0[];

extern s32 device_cmd_send();
extern s32 CDQ_init(void);
extern s32 cdq_command_process(void);
extern s32 SD_CmdProc(void);
extern s32 ResetGraph();
extern s32 SetDispMask();
extern s32 DrawSync();
extern s32 VSync();
extern s32 printf();
extern s32 EnterCriticalSection(void);
extern s32 ExitCriticalSection(void);
extern s32 GetSp(void);
extern s32 CloseEvent();
extern s32 tt_obj_disp(void);
extern s32 tt_load_wait(void);
extern s32 tt_flags_clear(void);
extern s32 obj_pool_disp(void);
extern s32 tt_sprite_pool_disp(void);
extern s32 tt_mode_disp(void);
extern s32 tt_timer_init(void);
extern s32 tt_buffers_clear(void);
extern s32 tt_full_clear(void);
extern s32 tt_config_init(void);
extern s32 tt_dbuf_init(void);
extern s32 tt_screen_clear(void);
extern s32 tt_pause_step(void);
extern s32 tt_hold_check(void);
extern s32 tt_dbuf_swap(void);
extern s32 tt_dbuf_show(void);
extern s32 tt_pad_read(void);
extern s32 tt_unit_disp(void);
extern s32 tt_resource_load(void);
extern s32 tt_pair_disp(void);
extern s32 tt_rec_disp(void);
extern s32 tt_ent_disp(void);
extern s32 tt_anim_disp(void);
extern s32 tt_text_disp(void);
extern s32 tt_time_set();
extern s32 tt_recs_disp(void);
extern s32 tt_sprite_disp(void);
extern s32 tt_slot_disp(void);
extern s32 tt_record_disp(void);

void tt_main(void)
{
    u8 *p;
    u8 *q;
    u8 *r;
    u8 *w;
    s32 flag;

    p = tt_game_wk;
    q = tt_step_wk;
    r = tt_scroll_wk;
    printf(tt_twinbee_start_fmt, GetSp());
    tt_full_clear();
    tt_config_init();
    tt_timer_init();
    tt_dbuf_init();
    tt_screen_clear();
    tt_resource_load();
    tt_load_wait();
    q[14] = 0;
    for (;;) {
        q[16] = 0;
        device_cmd_send(33);
        device_cmd_send(34);
        device_cmd_send(47);
        device_cmd_send(33);
        device_cmd_send(241);
        CDQ_init();
        for (;;) {
            tt_dbuf_swap();
            if (tt_hold_check() != 0) {
                if (p[83] != 0) {
                    goto restart;
                }
                *(s16 *)(q + 0) = 8;
                *(s16 *)(q + 6) = 0;
                *(s16 *)(q + 4) = 0;
                *(s16 *)(q + 2) = 0;
                q[11] = 0;
                q[10] = 0;
                q[9] = 0;
            }
            if (q[17] != 0) {
                tt_pause_step();
            }
            if (q[16] == 0) {
                tt_main_fn_tbl[*(u16 *)(q + 0)]();
                *(u16 *)(p + 10) = *(u16 *)(p + 10) + 1;
            }
            if (p[85] != 0) {
                goto finish;
            }
            tt_time_set(*(u16 *)(r + 8), 39);
            *(u16 *)(p + 8) = *(u16 *)(p + 8) + 1;
            tt_text_disp();
            tt_record_disp();
            tt_pair_disp();
            tt_anim_disp();
            tt_sprite_disp();
            tt_rec_disp();
            tt_recs_disp();
            tt_slot_disp();
            tt_sprite_pool_disp();
            tt_unit_disp();
            tt_ent_disp();
            tt_obj_disp();
            obj_pool_disp();
            tt_mode_disp();
            DrawSync(0);
            VSync(0);
            tt_pad_read();
            tt_dbuf_show();
            SD_CmdProc();
            cdq_command_process();
            tt_flags_clear();
        }
restart:
        tt_buffers_clear();
        q[14] = 0;
    }
finish:
    tt_config = *(u16 *)(p + 48);
    (*(TT_CONFIG *)&tt_config).name[0][0] = p[50];
    (*(TT_CONFIG *)&tt_config).name[0][1] = p[51];
    (*(TT_CONFIG *)&tt_config).name[0][2] = p[52];
    (*(TT_CONFIG *)&tt_config).score[1] = *(u16 *)(p + 54);
    (*(TT_CONFIG *)&tt_config).name[1][0] = p[56];
    (*(TT_CONFIG *)&tt_config).name[1][1] = p[57];
    (*(TT_CONFIG *)&tt_config).name[1][2] = p[58];
    (*(TT_CONFIG *)&tt_config).score[2] = *(u16 *)(p + 60);
    (*(TT_CONFIG *)&tt_config).name[2][0] = p[62];
    (*(TT_CONFIG *)&tt_config).name[2][1] = p[63];
    (*(TT_CONFIG *)&tt_config).name[2][2] = p[64];
    D_800F022B = p[68];
    D_800F022C = p[69];
    w = tt_disp_wk;
    if (p[83] != 0) {
        goto store;
    }
    if (p[82] != 0) {
        goto skip;
    }
store:
    (*(TT_CONFIG *)&tt_config).flags = 0;
    if (p[67] != 0) {
        flag = 1;
    } else {
        flag = 0;
    }
    (*(TT_CONFIG *)&tt_config).flags = tt_config_flags_b | flag;
    if (p[73] != 0) {
        flag = 2;
    } else {
        flag = 0;
    }
    (*(TT_CONFIG *)&tt_config).flags = tt_config_flags_u | flag;
skip:
    tt_result = r[11];
    EnterCriticalSection();
    CloseEvent(*(s32 *)w);
    printf(tt_open_event_fmt, *(s32 *)w);
    ExitCriticalSection();
    ResetGraph(3);
    SetDispMask(0);
    DrawSync(0);
    printf(tt_main_str0);
}
