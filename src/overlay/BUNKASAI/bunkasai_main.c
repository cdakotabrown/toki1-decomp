/* @RodataBase: 0x8015D78C */
/* Address: 0x80142700 */
/* Size: 1176 bytes (294 insns) */
/* @SymbolSize: 1176 */
/* @CompileEnv: IDO52_AS1_TIE_REJECT=127,131,135,137,138 */
#include "types.h"

typedef struct {
    char x[11];
} Hdr11;

extern Hdr11 bunkasai_main_tbl;

extern void goto_sub_sub_step(s32);
extern void sub_step_up(void);
extern void device_cmd_send(s32);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void k_reset(s32);
extern void dec_bg_reset(void);
extern void set_dec_bri(s32);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern void bustup_cd_read(s32);
extern void parameter_disp_switch(s32);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void icon_disp_switch(s32);
extern void basyo_init(void *);
extern void mes_window_switch(s32);
extern void season_bgm_set(void);
extern void mascot_disp_switch(s32);
extern void return_schedule(s32);
extern void fade_in_step(void);
extern void bustup_ani(void);
extern void date_name_set(s32);
extern void yazirushi_off(void);
extern void shared_system_state_reset(void);
extern void serifu_run(void);
extern void date_x_taku_set(s32, s32);
extern void common_disp(void);
extern void bunkasai_common_bank_set(void);
extern void bunkasai_state_up(void);
extern void bunkasai_girl_para_add(void);
extern void bunkasai_event_log_add(void);
extern void bunkasai_birth_main(void);
extern void bunkasai_sub_dispatch(void);

extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern u8  now_month;
extern u8  now_day;
extern s32 girl0_birth;
extern u8  day_flag;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern s32 bunkasai_serifu_ptr[];
extern char bunkasai_main_str0[];
typedef struct {
    s32 flags;
    u8 pad[52];
} GirlRec;

typedef struct {
    u8 pad[288];
    GirlRec girl[11];
} GirlBlock;

extern GirlBlock girl_blk;
extern u8 girl_appear;
extern u8 girl_appear_1;
extern u8 girl_appear_2;
extern u8 girl_appear_3[];

extern s32 work_miura[64];
#define common_disp_off work_miura[50]
#define bunka_girl2 work_miura[53]
#define bunka_girl work_miura[59]
#define bunka_state work_miura[60]

void bunkasai_main(void) {

    s32 v0;
    s32 i;
    Hdr11 hdr;

    hdr = bunkasai_main_tbl;
    bustup_ani();

    switch (bunka_state) {
    case 0:
        device_cmd_send(191);
        device_load_start(1, 0xC486, 0xC459, 0xD266, 0xD216, 0xD20E);
        date_x_taku_set(1, 0);
        shared_system_state_reset();
        girl_appear = (u32)(girl_blk.girl[0].flags << 30) >> 31;
        girl_appear_1 = (u32)(girl_blk.girl[1].flags << 30) >> 31;
        girl_appear_2 = (u32)(girl_blk.girl[2].flags << 30) >> 31;
        for (i = 0; i < 8; i++) {
            girl_appear_3[i] = (u32)(girl_blk.girl[i + 3].flags << 30) >> 31;
        }
        dec_bg_reset();
        message_window_init();
        k_reset(0);
        basyo_init(bunkasai_main_str0);
        mes_window_switch(0);
        mascot_disp_switch(0);
        hizuke_init();
        icon_disp_switch(0);
        parameter_disp_switch(0);
        bunka_girl = 255;
        bunka_girl2 = 255;
        common_disp_off = 0;
        bunka_state = bunka_state + 1;
        break;
    case 1:
        set_dec_bri(0);
        bunka_state = bunka_state + 1;
        break;
    case 2:
        if (device_load_poll() == 1) {
            device_cmd_send(512);
            value_cache_find_or_load(16425, 1);
            bunka_state = bunka_state + 1;
        }
        break;
    case 3:
        if (get_CD_State2() & 1) {
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            cd_read_start2(5, 0x801E8000, 18874);
            mes_window_switch(1);
            bunka_state = bunka_state + 1;
        }
        break;
    case 4:
        fade_in_step();
        bunkasai_state_up();
        break;
    case 5:
        if (get_CD_State2() & 1) {
            yazirushi_off();
            date_name_set(0);
            bunkasai_common_bank_set();
            now_serifu = 0;
            now_serifu_in = 0;
            ss_serifu = bunkasai_serifu_ptr[0];
            ss_serifu_kind = bunkasai_serifu_ptr[1];
            sss_sound = bunkasai_serifu_ptr[2];
            bunka_state = bunka_state + 1;
        }
        break;
    case 6:
        serifu_run();
        bunkasai_state_up();
        break;
    case 7:
        bunkasai_sub_dispatch();
        break;
    case 8:
        v0 = girl0_birth;
        if ((v0 & 0xF) == now_month &&
            ((u32)(v0 << 23) >> 27) == now_day &&
            (day_flag & 1) == 0) {
            bunka_state = 9;
            now_sub_step = 0;
            now_sub_sub_step = 0;
        } else if (now_sub_step == 0) {
            device_cmd_send(191);
            season_bgm_set();
            sub_step_up();
        } else if (device_load_poll() == 1) {
            bunkasai_girl_para_add();
            bunkasai_event_log_add();
            for (i = 0; i < 64; i++) {
                work_miura[i] = 0;
            }
            shared_system_state_reset();
            dec_bg_reset();
            set_dec_bri(0);
            return_schedule(1);
        }
        break;
    case 9:
        bunkasai_birth_main();
        break;
    default:
        game_reset();
        break;
    }
    if (common_disp_off == 0) {
        common_disp();
    }
}
