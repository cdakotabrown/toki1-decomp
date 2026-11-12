/* @RodataBase: 0x8013BFD4 */
/* @PostProc: swap_instructions --offset1 0xDC --offset2 0xE0 */
/* @PostProc: swap_instructions --offset1 0xE0 --offset2 0xE4 */
/* @PostProc: swap_instructions --offset1 0xE4 --offset2 0xE8 */
/* @PostProc: swap_instructions --offset1 0xE8 --offset2 0xEC */
/* @PostProc: swap_instructions --offset1 0xEC --offset2 0xF0 */
/* @PostProc: patch_insn_reg_field 0x90:rt:t1:t2 0x94:rs:t1:t2 0x94:rt:t1:t2 0x98:rt:t2:t1 0xa0:rd:t4:t3 0xa4:rt:t3:t5 0xa8:rs:t3:t5 0xa8:rt:t3:t5 0xac:rd:t5:t4 0xac:rt:t4:t3 0xb0:rs:t3:t4 0x19c:rt:t4:t3 0x1a0:rs:t4:t3 0x1a0:rt:t4:t3 0x1ac:rt:t3:t4 0x1b0:rs:t3:t4 0x1b0:rt:t3:t4 0x1b4:rt:t4:t3 0x1c4:rt:t3:t4 0x1fc:rt:v0:v1 0x200:rs:v0:v1 0x200:rt:v0:v1 0x208:rs:v0:v1 0x320:rt:t4:t3 0x324:rs:t4:t3 0x344:rt:t3:t4 0x350:rt:t3:t4 0x450:rt:t4:t3 0x45c:rt:t4:t3 0x460:rt:t3:t4 0x468:rt:t3:t4 0x48c:rt:v0:v1 0x490:rs:v0:v1 0x490:rt:v0:v1 0x498:rs:v0:v1 0x4a4:rs:v0:v1 0x4b0:rs:v0:v1 0x51c:rt:t4:t3 0x520:rs:t4:t3 0x520:rt:t4:t3 0x528:rs:t4:t3 0x528:rt:t3:t4 0x52c:rt:t3:t4 0x58c:rt:t4:t3 0x590:rt:t4:t3 0x5f8:rt:t3:t4 0x5fc:rs:t3:t4 0x5fc:rt:t3:t4 0x604:rs:t3:t4 0x778:rt:t4:t3 0x77c:rs:t4:t3 0x78c:rt:t3:t4 0x794:rt:t3:t4 0x84c:rt:v0:v1 0x850:rs:v0:v1 0x850:rt:v0:v1 0x858:rs:v0:v1 0x864:rs:v0:v1 0x870:rs:v0:v1 0x874:rt:t3:t4 0x8a0:rt:t4:t3 0x8a4:rs:t4:t3 0x8a4:rt:t4:t3 0x8ac:rs:t4:t3 0x8ac:rt:t3:t4 0x8b0:rt:t3:t4 0x910:rt:t4:t3 0x918:rt:t4:t3 0x920:rt:t3:t4 */
/* master_birth_day_main - MASTER overlay, Address: 0x80138490, Size: 2504 bytes (626 insns) */
#include "types.h"

extern u8  now_sub_sub_step;
extern volatile u8  schedule_cursor;
extern volatile u8  day_flag;
extern u32 p_birthday;
extern u8  now_month;
extern volatile u8  now_day;
extern u8  now_girl;
extern volatile s16 girl0_tokimeki;
extern volatile s16 girl0_yuukou;
extern volatile s16 girl0_syoushin;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern s32 master_serifu_ptr[];
extern s32 serifu_no;
extern s32 master_girl_res_tbl[][13];

extern void bustup_ani();
extern void device_cmd_send(s32);
extern void Vblnk_Timer_Init();
extern s32 get_g_zyotai_low();
extern void cd_read_start2();
extern void goto_sub_sub_step(s32);
extern void sub_sub_step_up(void);
extern s32 get_CD_State2(void);
extern void device_load_start();
extern s32 device_load_poll(void);
extern void serifu_run(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void sprite_fade_main(void);
extern void event_log_add();
extern void bustup_face_change();
extern void date_name_set(s32);
extern void master_res_tbl_init();
extern void bustup_cd_read(s32);
extern void bustup_ptr_init(void);
extern void zyotai_sprite_set_chk();
extern void menu2_main();
extern void menu3_main();
extern void master_choice_str_set(void);
extern void master_serifu_set();
extern void sub_step_up(void);
extern void game_reset(void);
extern void common_disp(void);
extern void bustup_kuchi_ani();

void master_birth_day_main(void) {
    u32 state;
    s32 sel;
    u32 pad;
    s32 v1;

    bustup_ani();
    state = now_sub_sub_step;
    switch (state) {
    case 0:
        schedule_cursor = (u8)(schedule_cursor + 1);
        day_flag = (u8)(day_flag | 1);
        device_cmd_send(207);
        Vblnk_Timer_Init();
        pad = p_birthday;
        if ((pad & 0xF) == now_month &&
            ((pad << 23) >> 27) == now_day &&
            !(day_flag & 4) &&
            (u32)get_g_zyotai_low(0) < 2) {
            day_flag = (u8)(day_flag | 4);
            cd_read_start2(3, (void *)0x801B0000, 0xAF43);
            goto_sub_sub_step(27);
        } else {
            cd_read_start2(3, (void *)0x801E8000, 18871);
            sub_sub_step_up();
        }
        break;
    case 1:
        sprite_fade_main();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            device_load_start(1, 0xC051, 0xC02B, 0xCAA3, 0xCA5D, 0xCA4C);
            now_girl = 0;
            date_name_set(0);
            master_serifu_set();
            now_serifu = 0;
            now_serifu_in = 0;
            ss_serifu = master_serifu_ptr[0];
            ss_serifu_kind = master_serifu_ptr[1];
            sss_sound = master_serifu_ptr[2];
            sub_sub_step_up();
        }
        break;
    case 3:
        serifu_run();
        break;
    case 4:
        menu2_main();
        break;
    case 5:
        if (serifu_no != 0) {
            serifu_run();
        } else {
            goto_sub_sub_step(7);
        }
        break;
    case 6:
        girl0_yuukou = (s16)(girl0_yuukou - 1);
        girl0_syoushin = (s16)(girl0_syoushin + 5);
        sub_step_up();
        break;
    case 7:
        if (device_load_poll() == 1) {
            device_cmd_send(512);
            bustup_cd_read(21533);
            now_serifu = 2;
            sub_sub_step_up();
        }
        break;
    case 8:
        serifu_run();
        break;
    case 9:
        now_serifu = 5;
        sub_sub_step_up();
        break;
    case 10:
        serifu_run();
        break;
    case 11:
        menu3_main();
        break;
    case 12:
        master_choice_str_set();
        now_serifu = 6;
        sub_sub_step_up();
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            zyotai_sprite_set_chk();
        }
        break;
    case 15:
        now_serifu = 8;
        sub_sub_step_up();
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        fade_up_main();
        break;
    case 18:
        sel = get_g_zyotai_low(0);
        switch ((u32)sel) {
        case 0:
        case 1:
            now_serifu = 15;
            break;
        case 2:
            now_serifu = 15;
            break;
        case 3:
            now_serifu = 16;
            break;
        case 4:
            now_serifu = 17;
            break;
        }
        sub_sub_step_up();
        break;
    case 19:
        serifu_run();
        break;
    case 20:
        sel = get_g_zyotai_low(0);
        switch ((u32)sel) {
        case 0:
        case 1:
            now_serifu = 23;
            break;
        case 2:
            now_serifu = 24;
            break;
        case 3:
            now_serifu = 25;
            break;
        case 4:
            now_serifu = 26;
            break;
        }
        sub_sub_step_up();
        break;
    case 21:
        serifu_run();
        break;
    case 22:
        v1 = serifu_no;
        switch (v1) {
        case 0:
            now_serifu = 27;
            bustup_face_change(1);
            girl0_tokimeki = (s16)(girl0_tokimeki + 3);
            girl0_yuukou = (s16)(girl0_yuukou + 2);
            girl0_syoushin = (s16)(girl0_syoushin - 15);
            break;
        case 1:
            now_serifu = 29;
            bustup_face_change(2);
            girl0_tokimeki = (s16)(girl0_tokimeki + 2);
            girl0_yuukou = (s16)(girl0_yuukou + 1);
            girl0_syoushin = (s16)(girl0_syoushin - 10);
            break;
        case 2:
            now_serifu = 31;
            bustup_face_change(3);
            girl0_yuukou = (s16)(girl0_yuukou + 1);
            girl0_syoushin = (s16)(girl0_syoushin + 10);
            break;
        }
        bustup_kuchi_ani();
        sub_sub_step_up();
        break;
    case 23:
        serifu_run();
        break;
    case 24:
        fade_down_main();
        break;
    case 25:
        serifu_run();
        break;
    case 26:
        event_log_add(21, 6);
        sub_step_up();
        break;
    case 27:
        schedule_cursor = (u8)(schedule_cursor + 1);
        sprite_fade_main();
        break;
    case 28:
        if (get_CD_State2() & 1) {
            now_girl = 0;
            date_name_set(0);
            master_res_tbl_init();
            now_serifu = 0;
            now_serifu_in = 0;
            ss_serifu = master_girl_res_tbl[0][1];
            ss_serifu_kind = master_girl_res_tbl[1][1];
            sss_sound = master_girl_res_tbl[2][1];
            bustup_cd_read(21533);
            sub_sub_step_up();
        }
        break;
    case 29:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            zyotai_sprite_set_chk();
            device_cmd_send(191);
            device_load_start(1, 0xC051, 0xC02B, 0xCAA3, 0xCA5D, 0xCA4C);
        }
        break;
    case 30:
        if (device_load_poll() == 1) {
            device_cmd_send(512);
            sub_sub_step_up();
        }
        break;
    case 31:
        serifu_run();
        break;
    case 32:
        fade_up_main();
        break;
    case 33:
        serifu_run();
        break;
    case 34:
        cd_read_start2(3, (void *)0x801E8000, 18871);
        sub_sub_step_up();
        break;
    case 35:
        if (get_CD_State2() & 1) {
            master_serifu_set();
            now_serifu = 12;
            now_serifu_in = 0;
            ss_serifu = master_serifu_ptr[0];
            ss_serifu_kind = master_serifu_ptr[1];
            sss_sound = master_serifu_ptr[2];
            sub_sub_step_up();
        }
        break;
    case 36:
        serifu_run();
        break;
    case 37:
        now_serifu = 33;
        sub_sub_step_up();
        break;
    case 38:
        serifu_run();
        break;
    case 39:
        menu3_main();
        break;
    case 40:
        now_serifu = 23;
        sub_sub_step_up();
        break;
    case 41:
        serifu_run();
        break;
    case 42:
        v1 = serifu_no;
        switch (v1) {
        case 0:
            now_serifu = 27;
            bustup_face_change(1);
            girl0_tokimeki = (s16)(girl0_tokimeki + 3);
            girl0_yuukou = (s16)(girl0_yuukou + 2);
            girl0_syoushin = (s16)(girl0_syoushin - 15);
            break;
        case 1:
            now_serifu = 29;
            bustup_face_change(2);
            girl0_tokimeki = (s16)(girl0_tokimeki + 2);
            girl0_yuukou = (s16)(girl0_yuukou + 1);
            girl0_syoushin = (s16)(girl0_syoushin - 10);
            break;
        case 2:
            now_serifu = 31;
            bustup_face_change(3);
            girl0_yuukou = (s16)(girl0_yuukou + 1);
            girl0_syoushin = (s16)(girl0_syoushin + 10);
            break;
        }
        bustup_kuchi_ani();
        sub_sub_step_up();
        break;
    case 43:
        serifu_run();
        break;
    case 44:
        fade_down_main();
        break;
    case 45:
        serifu_run();
        break;
    case 46:
        event_log_add(21, 6);
        sub_step_up();
        break;
    default:
        game_reset();
        break;
    }
    common_disp();
}
