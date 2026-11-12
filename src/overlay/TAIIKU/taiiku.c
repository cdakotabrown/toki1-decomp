/* @CompileEnv: IDO52_FORCE_WEB_COLOR=52:a0,17:a3 */
/* @RodataBase: 0x80148B00 */
/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic0 */
/* @PostProc: swap_instructions --offset1 0x26c --offset2 0x274 */
/* @PostProc: swap_instructions --offset1 0x2bc --offset2 0x2c4 */
/* @PostProc: patch_insn_reg_field 0x150:rd:a2:v1 0x160:rt:t0:a2 0x164:rt:a0:a1 0x168:rt:v1:a0 0x170:rs:t0:a2 0x170:rt:a2:v1 0x17c:rs:v1:a0 0x184:rs:a2:v1 0x184:rt:a2:v1 0x188:rs:a2:v1 0x190:rd:a2:v1 0x198:rt:a0:a1 0x1a0:rs:a2:v1 0x1d0:rt:a2:v1 0x1d4:rt:a2:v1 0x1ec:rt:a2:v1 0x1f0:rt:a2:v1 0x1f4:rt:t0:a2 0x1f8:rt:a2:v1 0x1fc:rs:a2:v1 0x20c:rs:t0:a2 0x21c:rt:a2:v1 0x220:rt:t0:a2 0x224:rt:a2:v1 0x234:rs:a2:v1 0x244:rs:t0:a2 0x248:rt:v1:a1 0x258:rt:v1:a1 0x26c:rt:a2:v1 0x274:rt:v1:a1 0x278:rt:a2:v1 0x27c:rt:t0:a2 0x280:rt:a2:v1 0x284:rt:v1:a1 0x288:rs:a2:v1 0x298:rs:t0:a2 0x2ac:rt:v1:a1 0x2b4:rs:v1:a1 0x2bc:rt:a2:v1 0x2c8:rt:a2:v1 0x2cc:rt:t0:a2 0x2d0:rt:a2:v1 0x2d8:rs:a2:v1 0x2e8:rs:t0:a2 0x2ec:rt:v1:a1 0x2f4:rt:v1:a1 0x2fc:rs:v1:a1 0x328:rt:v1:a1 0x338:rd:a1:a3 0x340:rt:a1:a3 0x34c:rd:a2:t9 0x350:rt:a2:t9 0x368:rt:a2:t8 0x370:rt:a1:a3 0x374:rt:a2:t8 0x378:rt:t1:t7 0x37c:rt:a3:a1 0x380:rs:a1:a3 0x380:rt:a1:a3 0x384:rs:a1:a3 0x384:rt:t1:t7 0x388:rd:t9:t1 0x38c:rt:t9:t1 0x390:rt:t8:a2 0x398:rt:t8:a2 0x3a0:rt:a3:a1 0x3bc:rt:t7:t6 0x3c0:rs:t7:t6 0x3f0:rt:t6:t2 0x3f4:rs:t6:t2 0x3f4:rt:t6:t2 0x3fc:rd:t2:t3 0x3fc:rt:t6:t2 0x400:rt:t2:t3 0x440:rt:t3:t4 0x444:rs:t3:t4 0x454:rt:t4:t5 0x458:rs:t4:t5 0x458:rt:t4:t5 0x464:rt:t4:t5 0x49c:rt:t5:t9 0x4a0:rs:t5:t9 0x4d4:rt:t9:t8 0x4d8:rs:t9:t8 0x4d8:rt:t9:t8 0x4e4:rt:t8:t1 0x4e8:rs:t8:t1 0x4e8:rt:t8:t1 0x4ec:rt:t9:t8 0x4f4:rt:t1:t7 0x4f8:rs:t1:t7 0x4f8:rt:t1:t7 0x4fc:rt:t8:t1 0x508:rt:t1:t7 0x53c:rd:t7:t6 0x544:rt:t7:t6 0x548:rt:t7:t6 0x550:rs:t7:t6 0x558:rt:t6:t2 0x55c:rs:t6:t2 0x55c:rt:t6:t2 0x564:rd:t2:t3 0x564:rt:t6:t2 0x568:rd:t2:t3 0x568:rs:t2:t3 0x568:rt:t6:t2 0x56c:rs:t2:t3 0x56c:rt:t3:t4 0x574:rt:t3:t4 0x578:rt:t4:t5 0x57c:rs:t4:t5 0x57c:rt:t4:t5 0x584:rd:t5:t9 0x584:rt:t4:t5 0x588:rd:t5:t9 0x588:rs:t5:t9 0x588:rt:t4:t5 0x58c:rs:t5:t9 0x58c:rt:t9:t8 0x594:rt:t9:t8 0x598:rt:t8:t1 0x59c:rs:t8:t1 0x59c:rt:t8:t1 0x5a4:rd:t1:t7 0x5a4:rt:t8:t1 0x5a8:rd:t1:t7 0x5a8:rs:t1:t7 0x5a8:rt:t8:t1 0x5ac:rs:t1:t7 0x5ac:rt:t7:t6 0x5b0:rt:t7:t6 0x5e4:rt:t6:t2 0x5ec:rt:t6:t2 0x624:rt:t2:t3 0x628:rs:t2:t3 0x628:rt:t2:t3 0x630:rd:t3:t4 0x630:rt:t2:t3 0x634:rt:t3:t4 0x65c:rt:t4:t5 0x660:rs:t4:t5 0x670:rt:t5:t9 0x674:rs:t5:t9 0x674:rt:t5:t9 0x680:rt:t5:t9 0x6a4:rd:t9:t8 0x6ac:rt:t9:t8 0x6b0:rt:t9:t8 0x6b8:rs:t9:t8 0x6c0:rt:t8:t1 0x6c4:rs:t8:t1 0x6c4:rt:t8:t1 0x6cc:rd:t1:t7 0x6cc:rt:t8:t1 0x6d0:rd:t1:t7 0x6d0:rs:t1:t7 0x6d0:rt:t8:t1 0x6d4:rs:t1:t7 0x6d4:rt:t7:t6 0x6dc:rt:t7:t6 0x6e0:rt:t6:t2 0x6e4:rs:t6:t2 0x6e4:rt:t6:t2 0x6ec:rd:t2:t3 0x6ec:rt:t6:t2 0x6f0:rd:t2:t3 0x6f0:rs:t2:t3 0x6f0:rt:t6:t2 0x6f4:rs:t2:t3 0x6f4:rt:t3:t4 0x6fc:rt:t3:t4 0x700:rt:t4:t5 0x704:rs:t4:t5 0x704:rt:t4:t5 0x70c:rd:t5:t9 0x70c:rt:t4:t5 0x710:rd:t5:t9 0x710:rs:t5:t9 0x710:rt:t4:t5 0x714:rs:t5:t9 0x714:rt:t9:t8 0x718:rt:t9:t8 0x74c:rt:t8:t1 0x750:rs:t8:t1 0x750:rt:t8:t1 0x758:rd:t1:t7 0x758:rt:t8:t1 0x75c:rt:t1:t7 0x784:rt:t7:t6 0x788:rs:t7:t6 0x798:rt:t6:t2 0x79c:rs:t6:t2 0x79c:rt:t6:t2 0x7a8:rt:t6:t2 0x7cc:rd:t2:t3 0x7d0:rd:t3:t4 0x7d0:rt:t2:t3 0x7d4:rs:t3:t4 0x7d4:rt:t4:t5 0x7dc:rs:t4:t5 0x830:rd:t5:t9 0x838:rt:t5:t9 0x83c:rt:t5:t9 0x844:rs:t5:t9 0x84c:rt:t9:t8 0x850:rs:t9:t8 0x850:rt:t9:t8 0x858:rd:t8:t1 0x858:rt:t9:t8 0x85c:rd:t8:t1 0x85c:rs:t8:t1 0x85c:rt:t9:t8 0x860:rs:t8:t1 0x860:rt:t1:t7 0x868:rt:t1:t7 0x86c:rt:t7:t6 0x870:rs:t7:t6 0x870:rt:t7:t6 0x878:rd:t6:t2 0x878:rt:t7:t6 0x87c:rd:t6:t2 0x87c:rs:t6:t2 0x87c:rt:t7:t6 0x880:rs:t6:t2 0x880:rt:t2:t3 0x888:rt:t2:t3 0x88c:rt:t3:t4 0x890:rs:t3:t4 0x890:rt:t3:t4 0x898:rd:t4:t5 0x898:rt:t3:t4 0x89c:rd:t4:t5 0x89c:rs:t4:t5 0x89c:rt:t3:t4 0x8a0:rs:t4:t5 0x8a0:rt:t5:t9 0x8a4:rt:t5:t9 0x8b8:rt:t9:t8 0x8c0:rt:t9:t8 0x904:rt:t8:t1 0x90c:rs:t8:t1 0x910:rd:t1:t7 0x914:rt:t1:t7 0x918:rt:t7:t6 0x920:rs:t7:t6 0x920:rt:t6:t2 0x928:rt:t6:t2 0x92c:rt:t2:t3 0x938:rt:t2:t3 0x93c:rt:t3:t4 0x944:rt:t3:t4 */
/* Address: 0x80132000  Target size: 2412 bytes (603 insns) */
#include "types.h"

extern void sub_sub_step_up();
extern void goto_sub_sub_step();
extern void device_cmd_send();
extern void device_load_start();
extern s32  device_load_poll();
extern void device_port_init();
extern s32  get_CD_State2();
extern void cd_read_start2();
extern void game_reset();
extern void class_clear_f();
extern void k_disp_start();
extern void k_reset();
extern void set_dec_bri();
extern void dec_bg_reset();
extern s32  indexed_byte_lsb_set();
extern void value_cache_find_or_load();
extern s32  dec_bg_init();
extern void bustup_cd_read();
extern void parameter_disp_switch();
extern void message_window_init();
extern void icon_disp_switch();
extern void mes_window_switch();
extern void basyo_init();
extern void bustup_ptr_init();
extern void fade_up_main();
extern void fade_down_main();
extern void fade_in_step();
extern void fade_out_step();
extern void bustup_face_change();
extern void zyotai_sprite_set_chk();
extern void bustup_ani();
extern void date_name_set();
extern void yazirushi_off();
extern void shared_system_state_reset();
extern void serifu_run();
extern void date_x_taku_set();
extern void common_disp();
extern s32  get_g_zyotai_low();
extern s32  rand();
extern void srand();
extern void taiiku_rireki_set();
extern void taiiku_ptr3_init();

typedef struct {
    s8  pad0[2];
    s16 f2;
    s8  pad4[2];
    s16 f6;
    s32 f8;
    s8  pad12[44];
} Player;

typedef struct { s32 w[11]; } Buf44;

extern Player screen_w[];
u8 taiiku_girl_ord[8] = {0};
extern Buf44 taiiku_bustup_cd_tbl;
extern u8  taiiku_str0;
extern s32 vblnk_counter;
extern u8  now_sub_sub_step;
extern u8  screen_bri;
extern u8  now_girl;
extern s32 taiiku_pick_cnt;
extern s32 taiiku_end;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern s32 taiiku_ord_serifu;
extern s32 taiiku_ord_serifu_1;
extern s32 taiiku_ord_serifu_2;

void taiiku(void) {
    s32 j;
    s32 k;
    s32 count;
    u8 cnt;
    volatile u8 a0h;
    volatile u8 a1h;
    u8 pick[12];
    s32 i;
    s32 idx;
    s32 pad;
    Buf44 buf;
    s32 a0v;
    s32 a1v;

    buf = taiiku_bustup_cd_tbl;
    bustup_ani();

    switch ((u32)now_sub_sub_step) {
    case 0:
        device_load_start(0, 0xBFA6, 0xBF87, 0xD720, 0xD6D1, 0xD6C5);
        date_x_taku_set(1, 0);
        dec_bg_reset();
        set_dec_bri(0);
        shared_system_state_reset();
        class_clear_f();
        k_reset(0);
        basyo_init((s32)&taiiku_str0);
        k_disp_start(1);
        mes_window_switch(0);
        parameter_disp_switch(0);
        icon_disp_switch(0);
        message_window_init();
        srand(vblnk_counter);
        taiiku_pick_cnt = 0;
        sub_sub_step_up();
        break;
    case 1:
        if (device_load_poll() != 1) {
            break;
        }
        cnt = 0;
        for (idx = 0; idx != 11; idx++) {
            if (((u32)(screen_w[idx + 8].f8 << 30) >> 31) == 1) {
                pick[cnt] = (u8)idx;
                cnt = cnt + 1;
            }
        }
        if (cnt < 3U) {
            taiiku_end = 2;
            break;
        }
        if (taiiku_pick_cnt == 0) {
            value_cache_find_or_load(16691, 1);
        }
        screen_bri = 128;
        a0v = pick[(u32)rand() % (u32)cnt];
        a0h = (u8)a0v;
        a1v = pick[(u32)rand() % (u32)cnt];
        a0v = a0h;
        if (taiiku_pick_cnt != 0) {
            a0v = pick[1];
            a1v = pick[2];
        } else {
            while (a0v == 0) {
                a1h = (u8)a1v;
                a0v = pick[(u32)rand() % (u32)cnt];
                a1v = a1h;
            }
            while (a0v == a1v || a1v == 0) {
                a0h = (u8)a0v;
                a1v = pick[(u32)rand() % (u32)cnt];
                a0v = a0h;
            }
            taiiku_pick_cnt = taiiku_pick_cnt + 1;
        }
        {
            u8 *q;
            taiiku_girl_ord[0] = 0;
            taiiku_girl_ord[1] = a0v;
            taiiku_girl_ord[2] = a1v;
        }
        count = rand() & 0x1F;
        for (k = 0; k < count; k++) {
            i = rand() % 3;
            j = rand() % 3;
            idx = taiiku_girl_ord[i];
            taiiku_girl_ord[i] = taiiku_girl_ord[j];
            taiiku_girl_ord[j] = (u8)idx;
        }
        sub_sub_step_up();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            taiiku_rireki_set();
            device_port_init(0, 512);
            a1v = dec_bg_init(1);
            indexed_byte_lsb_set(1, a1v);
            bustup_cd_read(buf.w[taiiku_girl_ord[0]]);
            mes_window_switch(1);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = taiiku_girl_ord[0];
            zyotai_sprite_set_chk();
        }
        break;
    case 5:
        cd_read_start2(2, 0x801E8000, 0xB6DC);
        sub_sub_step_up();
        break;
    case 6:
        if (get_CD_State2() & 1) {
            yazirushi_off();
            date_name_set(taiiku_girl_ord[0]);
            taiiku_ptr3_init();
            now_serifu = 0;
            now_serifu_in = 0;
            ss_serifu = taiiku_ord_serifu;
            ss_serifu_kind = taiiku_ord_serifu_1;
            sss_sound = taiiku_ord_serifu_2;
            sub_sub_step_up();
        }
        break;
    case 7: case 8: case 9: case 12: case 15: case 17:
    case 22: case 25: case 27: case 32: case 35: case 40:
        serifu_run();
        break;
    case 10:
        switch ((u32)get_g_zyotai_low(taiiku_girl_ord[0])) {
        case 0: case 1:
            now_serifu = (s16)(taiiku_girl_ord[0] * 3 + 5);
            break;
        case 2: case 3:
            now_serifu = (s16)(taiiku_girl_ord[0] * 3 + 6);
            break;
        case 4:
            now_serifu = (s16)(taiiku_girl_ord[0] * 3 + 7);
            break;
        }
        sub_sub_step_up();
        break;
    case 11: case 20: case 30:
        fade_up_main();
        break;
    case 13: case 23: case 33: case 38:
        fade_down_main();
        break;
    case 14: case 24:
        now_serifu = 1;
        now_serifu_in = 0;
        sub_sub_step_up();
        break;
    case 16:
        date_name_set(taiiku_girl_ord[1]);
        sub_sub_step_up();
        break;
    case 18:
        bustup_cd_read(buf.w[taiiku_girl_ord[1]]);
        sub_sub_step_up();
        break;
    case 19:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = taiiku_girl_ord[1];
            zyotai_sprite_set_chk();
        }
        break;
    case 21:
        switch ((u32)get_g_zyotai_low(taiiku_girl_ord[1])) {
        case 0: case 1:
            now_serifu = (s16)(taiiku_girl_ord[1] * 3 + 5);
            break;
        case 2: case 3:
            now_serifu = (s16)(taiiku_girl_ord[1] * 3 + 6);
            break;
        case 4:
            now_serifu = (s16)(taiiku_girl_ord[1] * 3 + 7);
            break;
        }
        sub_sub_step_up();
        break;
    case 26:
        date_name_set(taiiku_girl_ord[2]);
        sub_sub_step_up();
        break;
    case 28:
        bustup_cd_read(buf.w[taiiku_girl_ord[2]]);
        sub_sub_step_up();
        break;
    case 29:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = taiiku_girl_ord[2];
            zyotai_sprite_set_chk();
        }
        break;
    case 31:
        if (screen_w[taiiku_girl_ord[2] + 8].f6 >= 76 ||
            get_g_zyotai_low(taiiku_girl_ord[2]) == 4) {
            device_cmd_send(191);
            bustup_face_change(4);
            goto_sub_sub_step(38);
        } else {
            switch ((u32)get_g_zyotai_low(taiiku_girl_ord[2])) {
            case 0: case 1:
                now_serifu = (s16)(taiiku_girl_ord[2] * 3 + 5);
                break;
            case 2: case 3:
                now_serifu = (s16)(taiiku_girl_ord[2] * 3 + 6);
                break;
            case 4:
                now_serifu = (s16)(taiiku_girl_ord[2] * 3 + 7);
                break;
            }
            sub_sub_step_up();
        }
        break;
    case 34:
        now_serifu = 3;
        now_serifu_in = 0;
        sub_sub_step_up();
        break;
    case 36: case 41:
        fade_out_step();
        break;
    case 37: case 42:
        for (i = 0; i < 3; i++) {
            screen_w[taiiku_girl_ord[i] + 8].f2 =
                (s16)(screen_w[taiiku_girl_ord[i] + 8].f2 + 1);
        }
        taiiku_end = 2;
        break;
    case 39:
        now_serifu = 4;
        now_serifu_in = 0;
        sub_sub_step_up();
        break;
    default:
        game_reset();
        break;
    }
    common_disp();
}
