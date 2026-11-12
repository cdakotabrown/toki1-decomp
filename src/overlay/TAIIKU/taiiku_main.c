/* @CompileFlags: -Wc,-pic0 */
/* @RodataBase: 0x80148F38 */
/* @PostProc: patch_insn_reg_field 0x2ec:rt:t6:t9 0x2f4:rs:t6:t9 0x2f4:rt:t9:t1 0x2f8:rs:t9:t1 0x300:rt:t1:t2 0x308:rt:t1:t2 0x310:rt:t2:t3 0x314:rs:t2:t3 0x314:rt:t2:t3 0x320:rt:t3:t4 0x324:rs:t3:t4 0x324:rt:t3:t4 0x328:rt:t2:t3 0x330:rt:t4:t5 0x334:rs:t4:t5 0x334:rt:t4:t5 0x338:rt:t3:t4 0x340:rt:t4:t5 0x35c:rt:t5:t8 0x360:rs:t5:t8 0x360:rt:t5:t8 0x364:rt:t8:t7 0x368:rs:t5:t8 0x370:rt:t7:t6 0x374:rs:t7:t6 0x374:rt:t7:t6 0x378:rt:t8:t7 0x380:rs:t7:t6 0x380:rt:t6:t9 0x384:rt:t6:t9 0x3a8:rt:t9:t1 0x3ac:rs:t9:t1 0x3ac:rt:t9:t1 0x3b4:rs:t9:t1 0x3d0:rt:t1:t2 0x3d4:rt:t1:t2 0x3f0:rt:t2:t3 0x3f4:rs:t2:t3 0x3f4:rt:t2:t3 0x3f8:rt:v0:v1 0x3fc:rs:t2:t3 0x404:rs:v0:v1 0x404:rt:v0:v1 0x408:rt:t3:t4 0x40c:rs:v0:v1 0x418:rs:v0:v1 0x41c:rt:t4:t5 0x424:rs:v0:v1 0x438:rt:t3:t4 0x444:rt:t4:t5 0x480:rt:t5:t8 0x484:rs:t5:t8 0x484:rt:t5:t8 0x488:rt:t8:t7 0x48c:rs:t5:t8 0x494:rt:t8:t7 0x4b8:rt:t7:t6 0x4bc:rs:t7:t6 0x4bc:rt:t7:t6 0x4c0:rt:v0:v1 0x4c4:rs:t7:t6 0x4cc:rs:v0:v1 0x4cc:rt:v0:v1 0x4d0:rt:t6:t9 0x4d4:rs:v0:v1 0x4e0:rs:v0:v1 0x4e4:rt:t9:t1 0x4ec:rs:v0:v1 0x500:rt:t6:t9 0x50c:rt:t9:t1 0x548:rt:t1:t2 0x54c:rs:t1:t2 0x54c:rt:t1:t2 0x550:rt:t2:t3 0x554:rs:t1:t2 0x55c:rt:t2:t3 0x580:rt:t3:t4 0x584:rs:t3:t4 0x584:rt:t3:t4 0x58c:rs:t3:t4 0x590:rt:t4:t5 0x59c:rt:t4:t5 0x5a8:rt:t5:t8 0x5ac:rs:t5:t8 0x5ac:rt:t5:t8 0x5b4:rs:t5:t8 0x5b8:rt:t8:t7 0x5c4:rt:t8:t7 0x5d0:rt:t7:t6 0x5d4:rs:t7:t6 0x5d4:rt:t7:t6 0x5dc:rs:t7:t6 0x5e0:rt:t6:t9 0x5ec:rt:t6:t9 0x5f8:rt:t9:t1 0x5fc:rs:t9:t1 0x5fc:rt:t9:t1 0x604:rs:t9:t1 0x608:rt:t1:t2 0x614:rt:t1:t2 0x620:rt:t2:t3 0x624:rs:t2:t3 0x624:rt:t2:t3 0x62c:rs:t2:t3 0x630:rt:t3:t4 0x634:rs:t3:t4 0x634:rt:t3:t4 0x63c:rs:t3:t4 0x63c:rt:t4:t5 0x644:rt:t4:t5 0x660:rt:t5:t8 0x664:rs:t5:t8 0x664:rt:t5:t8 0x66c:rs:t5:t8 0x66c:rt:t8:t7 0x674:rt:t8:t7 0x69c:rt:v0:v1 0x6a0:rt:v1:v0 0x6a4:rs:v1:v0 0x6a4:rt:v1:v0 0x6a8:rs:v0:v1 0x6a8:rt:v0:v1 0x6ac:rs:v0:v1 0x6ac:rt:v0:v1 0x6b0:rs:v0:v1 0x6b4:rs:v0:v1 0x6b8:rs:v0:v1 0x6bc:rs:v0:v1 0x6bc:rt:v1:v0 0x6c0:rs:v0:v1 */
/* Address: 0x80141450 */
/* Size: 1764 bytes (441 insns) */
#include "types.h"

typedef struct {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
    s32 unk10;
    s32 unk14;
} Blk24;

extern Blk24 taiiku_com_tpg_src;
s32 taiiku_com_tpg[6] = {0, 0, 0, 0, 0, 0};

extern s32 taiiku_state;
extern s32 taiiku_end;
extern s32 serifu_no;
extern s32 work_miura[64];

extern u8 now_sub_sub_step;
extern u8 byouki_kind;

extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;

extern s32 taiiku_main_serifu;
extern s32 taiiku_main_serifu_1;
extern s32 taiiku_main_serifu_2;

extern u8 taiiku_main_str0[];

extern void device_cmd_send();
extern void device_load_start();
extern s32 device_load_poll();
extern s32 get_CD_State2();
extern void cd_read_start2();
extern void game_reset();
extern void k_reset();
extern void set_dec_bri();
extern void dec_bg_reset();
extern void indexed_byte_lsb_set();
extern void value_cache_find_or_load();
extern s32 dec_bg_init();
extern void parameter_disp_switch();
extern void hizuke_init();
extern void message_window_init();
extern void icon_disp_switch();
extern void mes_window_switch();
extern void basyo_init();
extern void season_bgm_set();
extern void mascot_disp_switch();
extern void return_schedule();
extern void fade_in_step();
extern void date_name_set();
extern void yazirushi_off();
extern void shared_system_state_reset();
extern void serifu_run_end();
extern void serifu_run();
extern void date_x_taku_set();
extern void common_disp();
extern void taiiku();
extern void taiiku_ptrs_set();
extern void taiiku_sub_dispatch();
extern void taiiku_sub_dispatch0f();
extern void taiiku_menu_init();
extern void taiiku_menu();
extern void taiiku_menu_next();
extern void taiiku_menu_back();
extern void taiiku_bufptr_init3();
extern void taiiku_sub_dispatch0e();
extern void taiiku_sub_dispatch0c();

void taiiku_main(void) {

    s32 unusedHi[2];
    s32 buf[6];
    s32 unusedLo[2];
    s32 i;
    s32 temp;

    *(Blk24 *)buf = taiiku_com_tpg_src;

    switch ((u32)taiiku_state) {
    case 0:
        device_cmd_send(191);
        device_load_start(1, 0xC5C5, 0xC5A2, 0xD6C0, 0xD672, 0xD651);
        date_x_taku_set(1, 0);
        taiiku_end = 0;
        shared_system_state_reset();
        for (i = 0; i < 6; i++) {
            taiiku_com_tpg[i] = buf[i];
        }
        dec_bg_reset();
        message_window_init();
        k_reset(0);
        basyo_init(taiiku_main_str0);
        mes_window_switch(0);
        mascot_disp_switch(0);
        hizuke_init();
        icon_disp_switch(0);
        parameter_disp_switch(0);
        common_disp();
        taiiku_state = taiiku_state + 1;
        break;

    case 1:
        set_dec_bri(0);
        taiiku_state = taiiku_state + 1;
        common_disp();
        break;

    case 2:
        if (device_load_poll() == 1) {
            device_cmd_send(514);
            value_cache_find_or_load(16416, 1);
            taiiku_state = taiiku_state + 1;
        }
        common_disp();
        break;

    case 3:
        if ((get_CD_State2() & 1) != 0) {
            temp = dec_bg_init(1);
            indexed_byte_lsb_set(1, temp);
            cd_read_start2(1, 0x801F0000, 0xBA1D);
            mes_window_switch(1);
            taiiku_state = taiiku_state + 1;
        }
        common_disp();
        break;

    case 4:
        fade_in_step();
        if (now_sub_sub_step != 0) {
            now_sub_sub_step = 0;
            taiiku_state = taiiku_state + 1;
        }
        common_disp();
        break;

    case 5:
        if ((get_CD_State2() & 1) != 0) {
            taiiku_state = taiiku_state + 1;
            yazirushi_off();
            date_name_set(0);
            taiiku_ptrs_set();
            taiiku_bufptr_init3();
            now_serifu = 0;
            if ((byouki_kind & 2) != 0) {
                now_serifu = 6;
            }
            now_serifu_in = 0;
            ss_serifu = taiiku_main_serifu;
            ss_serifu_kind = taiiku_main_serifu_1;
            sss_sound = taiiku_main_serifu_2;
        }
        common_disp();
        break;

    case 6:
        serifu_run();
        if (now_sub_sub_step != 0) {
            now_serifu = 1;
            taiiku_state = taiiku_state + 1;
            now_sub_sub_step = 0;
        }
        common_disp();
        break;

    case 7:
        serifu_run_end();
        if (now_sub_sub_step != 0) {
            taiiku_menu_init();
            now_sub_sub_step = 0;
            taiiku_state = 8;
        }
        common_disp();
        break;

    case 8:
        taiiku_menu();
        if (now_sub_sub_step != 0) {
            switch (serifu_no) {
            case 0:
                taiiku_state = 12;
                break;
            case 1:
                taiiku_state = 13;
                break;
            case 2:
                taiiku_menu_next();
                break;
            default:
                game_reset();
                break;
            }
            now_sub_sub_step = 0;
        }
        common_disp();
        break;

    case 9:
        serifu_run_end();
        if (now_sub_sub_step != 0) {
            taiiku_state = 10;
            now_sub_sub_step = 0;
        }
        common_disp();
        break;

    case 10:
        taiiku_menu();
        if (now_sub_sub_step != 0) {
            switch (serifu_no) {
            case 0:
                taiiku_state = 14;
                break;
            case 1:
                taiiku_state = 15;
                break;
            case 2:
                taiiku_menu_back();
                break;
            default:
                game_reset();
                break;
            }
            now_sub_sub_step = 0;
        }
        common_disp();
        break;

    case 11:
        serifu_run_end();
        if (now_sub_sub_step != 0) {
            taiiku_state = 8;
            now_sub_sub_step = 0;
        }
        common_disp();
        break;

    case 12:
        taiiku_sub_dispatch0c();
        if (taiiku_end == 1) {
            taiiku_state = 16;
        }
        break;

    case 13:
        taiiku_sub_dispatch();
        if (taiiku_end == 1) {
            taiiku_state = 16;
        }
        break;

    case 14:
        taiiku_sub_dispatch0e();
        if (taiiku_end == 1) {
            taiiku_state = 16;
        }
        break;

    case 15:
        taiiku_sub_dispatch0f();
        if (taiiku_end == 1) {
            taiiku_state = 16;
        }
        break;

    case 16:
        taiiku();
        if (taiiku_end == 2) {
            taiiku_state = taiiku_state + 1;
        }
        break;

    case 17:
        device_cmd_send(191);
        season_bgm_set();
        common_disp();
        taiiku_state = taiiku_state + 1;
        break;

    case 18:
        if (device_load_poll() == 1) {
            dec_bg_reset();
            set_dec_bri(0);
            for (i = 0; i < 64; i++) {
                work_miura[i] = 0;
            }
            return_schedule(1);
        }
        common_disp();
        break;

    default:
        game_reset();
        break;
    }
}
