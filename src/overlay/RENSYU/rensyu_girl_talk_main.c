/* @SymbolSize: 1184 */
/* @DataBase: 0x8013431C */
/* @RodataBase: 0x80134154 */
/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    s16 v[13];
} Tbl13;

typedef struct {
    u8 pad0[18];
    u8 kind;
    u8 pad13[56 - 19];
} Rec;

typedef struct {
    u8 pad0[244];
    u16 date;
    u8 pad_f6[440 - 246];
    Rec rec[12];
} State;

extern State screen_w;
extern u8 now_sub_sub_step;
extern u8 now_girl;
extern u8 rensyu_girl_no;
extern s32 rensyu_serifu_ptr;
extern s32 rensyu_serifu_kind_ptr;
extern s32 rensyu_sound_ptr;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern s16 now_serifu;
extern s16 now_serifu_in;

extern void bustup_ani();
extern void cd_read_start2();
extern void sub_sub_step_up();
extern void serifu_run();
extern void device_port_init();
extern void date_name_set();
extern void rensyu_serifu_tbl_set();
extern void bustup_ptr_init();
extern void zyotai_sprite_set_chk();
extern void bustup_face_change();
extern void device_cmd_send();
extern void don_init();
extern void fade_up_main();
extern void fade_down_main();
extern void Vblnk_Timer_Init();
extern void fade_out_step();
extern void fade_out();
extern void event_log_add();
extern void mes_window_switch();
extern void sub_step_up();
extern void game_reset();
extern void common_disp();

void rensyu_girl_talk_main()
{
    s32 unused;
    Tbl13 tbl = { {
        41, 45, 49, 53, 57, 63, 67, 72, 76, 80, 84, 88, 90
    } };
    u8 msg;

    bustup_ani();
    switch (now_sub_sub_step) {
    case 0:
        if (device_load_poll() == 1) {
            cd_read_start2(7, 0x801E8000, 0xB037);
            sub_sub_step_up();
        }
        break;
    case 2:
    case 4:
    case 10:
        serifu_run();
        break;
    case 1:
        if (get_CD_State2() & 1) {
            device_port_init(1, 0);
            date_name_set(rensyu_girl_no);
            rensyu_serifu_tbl_set();
            ss_serifu = rensyu_serifu_ptr;
            ss_serifu_kind = rensyu_serifu_kind_ptr;
            sss_sound = rensyu_sound_ptr;
            now_serifu = tbl.v[rensyu_girl_no];
            now_serifu_in = 0;
            bustup_ptr_init();
            now_girl = rensyu_girl_no;
            zyotai_sprite_set_chk();
            if (rensyu_girl_no == 12) {
                bustup_face_change(0);
            }
            if (rensyu_girl_no == 11) {
                device_cmd_send(1280);
                don_init();
            }
        }
        break;
    case 3:
        fade_up_main();
        break;
    case 5:
        if (rensyu_girl_no == 11) {
            fade_down_main();
            now_serifu = 94;
        } else {
            now_serifu = 16;
            sub_sub_step_up();
        }
        break;
    case 6:
        device_cmd_send(193);
        Vblnk_Timer_Init();
        sub_sub_step_up();
        break;
    case 7:
        if (rensyu_girl_no == 11) {
            serifu_run();
        } else {
            sub_sub_step_up();
        }
        break;
    case 8:
        if (rensyu_girl_no == 11) {
            fade_out_step();
        } else {
            fade_out();
        }
        break;
    case 9:
        device_port_init(0, 6);
        if (rensyu_girl_no == 11) {
            sub_sub_step_up();
            sub_sub_step_up();
        } else {
            sub_sub_step_up();
        }
        break;
    case 11:
        switch ((screen_w.date >> 12) - 6) {
        case 0:
            if ((screen_w.rec[0].kind >> 4 == 6 && rensyu_girl_no == 0) ||
                (screen_w.rec[4].kind >> 4 == 6 && rensyu_girl_no == 4)) {
                msg = 5;
            } else {
                msg = 0;
            }
            break;
        case 1:
            if ((screen_w.rec[0].kind >> 4 == 7 && rensyu_girl_no == 0) ||
                (screen_w.rec[4].kind >> 4 == 7 && rensyu_girl_no == 4)) {
                msg = 6;
            } else {
                msg = 1;
            }
            break;
        case 2:
            if (rensyu_girl_no == 5) {
                msg = 7;
            } else {
                msg = 2;
            }
            break;
        case 3:
            if (rensyu_girl_no == 6) {
                msg = 8;
            } else {
                msg = 3;
            }
            break;
        case 4:
            if ((screen_w.rec[0].kind >> 4 == 10 && rensyu_girl_no == 0) ||
                (screen_w.rec[10].kind >> 4 == 10 && rensyu_girl_no == 10)) {
                msg = 9;
            } else {
                msg = 4;
            }
            break;
        }
        event_log_add(18, msg);
        mes_window_switch(0);
        sub_step_up();
        break;
    default:
        game_reset();
        break;
    }
    common_disp();
}
