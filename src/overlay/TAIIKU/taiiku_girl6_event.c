/* Address: 0x80141EAC */
/* Size: 1120 bytes (280 insns) */
/* @SymbolSize: 1120 */
/* @RodataBase: 0x80148F98 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0x360 --offset2 0x364 */
/* @PostProc: patch_insn_reg_field 0x24:rt:v1:t6 0x2c:rs:v1:t6 0x34:rd:t7:t6 0x34:rt:v1:t6 0x3c:rt:t7:t6 0x40:rt:t7:t6 0x48:rs:t7:t6 0x50:rt:t8:t7 0x54:rt:t9:t8 0x58:rt:t9:t8 0x5c:rt:t8:t7 0x10c:rt:t0:t9 0x110:rs:t0:t9 0x14c:rt:t1:t0 0x150:rs:t1:t0 0x160:rt:t2:t1 0x16c:rt:t2:t1 0x194:rt:t3:t2 0x198:rs:t3:t2 0x1c4:rt:t4:t3 0x1c8:rs:t4:t3 0x1c8:rt:t4:t3 0x1d4:rt:t5:t4 0x1d8:rs:t5:t4 0x1d8:rt:t5:t4 0x1dc:rt:t4:t3 0x1e4:rt:t6:t5 0x1e8:rs:t6:t5 0x1e8:rt:t6:t5 0x1ec:rt:t5:t4 0x1f8:rt:t6:t5 0x234:rt:t7:t6 0x238:rs:t7:t6 0x238:rt:t8:t6 0x240:rs:t8:t6 0x2c4:rt:t9:t7 0x2c8:rs:t9:t7 0x2f4:rt:t0:t8 0x2f8:rt:t1:t9 0x2fc:rs:t1:t9 0x2fc:rt:t1:t9 0x300:rs:t0:t8 0x300:rt:t0:t8 0x324:rt:t1:t9 0x32c:rt:t0:t8 0x368:rt:t2:t0 0x370:rs:t2:t0 0x370:rt:t3:t1 0x378:rt:t3:t1 0x3a0:rt:t4:t2 0x3a4:rs:t4:t2 0x408:rt:t5:t3 0x410:rt:t5:t3 0x420:rt:t6:t4 0x424:rs:t6:t4 0x424:rt:t6:t4 0x42c:rs:t6:t4 0x42c:rt:t7:t5 0x430:rt:t7:t5 0x434:rt:t8:t6 0x438:rs:t8:t6 0x438:rt:t8:t6 0x440:rs:t8:t6 0x440:rt:t9:t7 0x448:rt:t9:t7 */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 palette_upload_enqueue();
extern s32 device_cmd_send();
extern s32 device_load_start();
extern s32 device_load_poll();
extern s32 device_port_init();
extern s32 get_CD_State2();
extern s32 cd_read_start2();
extern s32 game_reset();
extern s32 don_init();
extern s32 tpage_buf_clear();
extern s32 class_clear_f();
extern s32 text_sys_init();
extern s32 k_reset();
extern s32 check_end_k();
extern s32 dec_bg_reset();
extern s32 indexed_byte_lsb_set();
extern s32 value_cache_find_or_load();
extern s32 dec_bg_init();
extern s32 bustup_cd_read();
extern s32 message_window_init();
extern s32 mes_window_switch();
extern s32 basyo_init();
extern s32 bustup_ptr_init();
extern s32 fade_up_main();
extern s32 fade_in_step();
extern s32 fade_out();
extern s32 zyotai_sprite_set_chk();
extern s32 bustup_ani();
extern s32 date_name_set();
extern s32 yazirushi_off();
extern s32 date_sprite_init();
extern s32 date_object_set();
extern s32 date_fade_in();
extern s32 date_fade_out();
extern s32 shared_system_state_reset();
extern s32 serifu_run();
extern s32 event_log_add();
extern s32 common_disp();
extern s32 taiiku_serifu_ptrs_set();
extern s32 taiiku_tpage_init();

extern u8 taiiku_girl6_event_str0[];
extern u8 class_w[];
extern u8 cw2_active[];
extern volatile s32 taiiku_girl6_res;
extern s32 taiiku_girl6_serifu;
extern s32 taiiku_girl6_serifu_1;
extern s32 taiiku_girl6_serifu_2;
extern u8 fade_bri;
extern u8 now_girl;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern s32 taiiku_girl6_dobj[];
extern volatile s16 girl6_tokimeki;
extern volatile s16 girl6_yuukou;

s32 taiiku_girl6_event(void)
{
    u8 sel;
    u8 *p;

    bustup_ani();
    sel = *(volatile u8 *)0x800EF86A;
    switch ((u32)sel) {
    case 0:
        device_load_start(0, 0xBFA6, 0xBF87, 0xD720, 0xD6D1, 0xD6C5);
        taiiku_girl6_res = 0;
        shared_system_state_reset();
        text_sys_init();
        tpage_buf_clear();
        dec_bg_reset();
        class_clear_f();
        k_reset(0);
        basyo_init(taiiku_girl6_event_str0);
        mes_window_switch(0);
        message_window_init();
        fade_bri = 0;
        sub_sub_step_up();
        break;
    case 1:
        if (device_load_poll() == 1) {
            value_cache_find_or_load(16497, 1);
            sub_sub_step_up();
        }
        break;
    case 2:
        if (get_CD_State2() & 1) {
            indexed_byte_lsb_set(1, dec_bg_init(1));
            bustup_cd_read(25718);
            sub_sub_step_up();
        }
        break;
    case 3:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 6;
            zyotai_sprite_set_chk();
            cd_read_start2(1, 0x801E8000, 0xB7C6);
        }
        break;
    case 4:
        if (get_CD_State2() & 1) {
            yazirushi_off();
            date_name_set(6);
            taiiku_serifu_ptrs_set();
            now_serifu = 0;
            now_serifu_in = 0;
            ss_serifu = taiiku_girl6_serifu;
            ss_serifu_kind = taiiku_girl6_serifu_1;
            sss_sound = taiiku_girl6_serifu_2;
            sub_sub_step_up();
        }
        break;
    case 5:
        if (check_end_k() != 0) {
            mes_window_switch(1);
        }

    case 7:
    case 9:
    case 10:
    case 12:
    case 18:
        serifu_run();
        break;
    case 6:
        fade_in_step();
        if (*(volatile u8 *)0x800EF86A == 7) {
            device_port_init(0, 514);
        }
        break;
    case 8:
        device_cmd_send(1546);
        don_init();
        sub_sub_step_up();
        break;
    case 11:
        fade_up_main();
        break;
    case 13:
        cd_read_start2(61, 0x80197000, 0xBB68);
        sub_sub_step_up();
        break;
    case 14:
        fade_out();
        break;
    case 15:
        if (get_CD_State2() & 1) {
            taiiku_tpage_init();
            palette_upload_enqueue(taiiku_girl6_dobj[4], 17, 1, 2, 0);
            date_sprite_init(taiiku_girl6_dobj[5], taiiku_girl6_dobj[6], taiiku_girl6_dobj[7],
                          taiiku_girl6_dobj[8], taiiku_girl6_dobj[9], taiiku_girl6_dobj[10]);
            date_object_set(taiiku_girl6_dobj[1], taiiku_girl6_dobj[2], taiiku_girl6_dobj[0], (*(s16 *)&taiiku_girl6_dobj[3]));
            p = class_w;
            do {
                p[6532] |= 8;
                p += 68;
            } while (p != cw2_active);
            value_cache_find_or_load(18216, 1);
            sub_sub_step_up();
        }
        break;
    case 16:
        if (get_CD_State2() & 1) {
            indexed_byte_lsb_set(1, dec_bg_init(1));
            sub_sub_step_up();
        }
        break;
    case 17:
        date_fade_in();
        break;
    case 19:
        device_cmd_send(209);
        sub_sub_step_up();
        break;
    case 20:
        date_fade_out();
        break;
    case 21:
        taiiku_girl6_res = 1;
        event_log_add(2, 45);
        girl6_yuukou = girl6_yuukou + 2;
        girl6_tokimeki = girl6_tokimeki + 1;
        break;
    default:
        game_reset();
        break;
    }
    common_disp();
    return taiiku_girl6_res;
}
