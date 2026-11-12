/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0x4EC --offset2 0x4F8 */
/* @PostProc: swap_instructions --offset1 0x4F4 --offset2 0x4F8 */
/* @PostProc: patch_insn_reg_field 0x224:rt:v0:v1 0x228:rs:v0:v1 0x228:rt:v0:v1 0x230:rs:v0:v1 0x238:rs:v0:v1 0x48c:rt:a0:v1 0x490:rt:v1:v0 0x494:rs:a0:v0 0x494:rt:a0:v0 0x49c:rt:v0:t8 0x4a4:rs:v0:t8 0x4a4:rt:t8:t9 0x4a8:rt:a0:v0 0x4ac:rt:t8:t9 0x4b0:rt:t0:t1 0x4b4:rs:t0:t1 0x4b4:rt:t0:t1 0x4b8:rt:t9:t0 0x4bc:rs:t9:t0 0x4bc:rt:t9:t0 0x4c0:rs:t0:t1 0x4c0:rt:t2:t4 0x4c4:rs:t0:t1 0x4c8:rs:t0:t1 0x4c8:rt:t0:t1 0x4cc:rs:t9:t0 0x4d0:rs:t0:t1 0x4d4:rs:t9:t0 0x4d4:rt:t9:t0 0x4d8:rs:t9:t0 0x4dc:rs:t0:t1 0x4e0:rs:t0:t1 0x4e0:rt:t2:t4 0x4e4:rs:t9:t0 0x4e8:rs:t0:t1 0x4ec:rt:t3:t5 0x4f0:rs:t9:t0 0x4f4:rs:t0:t1 0x4f4:rt:t2:t4 0x4fc:rs:t9:t0 0x4fc:rt:t2:t4 0x504:rt:t3:t5 0x5b8:rt:t4:t6 0x5bc:rs:t4:t6 0x5f0:rt:t5:t7 0x5f4:rs:t5:t7 0x5f4:rt:t5:t7 0x604:rs:t5:t7 0x604:rt:t6:t8 0x608:rt:t6:t8 0x610:rt:t7:t9 0x618:rt:t7:t9 0x61c:rs:t7:t9 0x624:rt:t8:v1 0x630:rt:t8:v1 0x648:rt:t1:t3 0x668:rt:t1:t3 0x6a8:rt:t0:t4 0x6c8:rt:t0:t4 0x6cc:rt:t9:t1 0x6d4:rt:t9:t1 0x6dc:rt:t3:t0 0x6e0:rt:t3:t0 0x724:rt:t4:t5 0x744:rt:t4:t5 0x770:rt:t5:t6 0x774:rt:t5:t6 0x778:rs:t5:t6 0x78c:rt:t6:t7 0x794:rt:t6:t7 0x7a4:rt:t7:t8 0x7a8:rs:t7:t8 0x7a8:rt:t7:t8 0x7b0:rs:t7:t8 0x7b0:rt:t8:t9 0x7b4:rt:t8:t9 0x7b8:rt:t1:t3 0x7bc:rs:t1:t3 0x7bc:rt:t1:t3 0x7c4:rs:t1:t3 */
/* @RodataBase: 0x80148E8C */
/* Address: 0x80140C60 */
/* Size: 2020 bytes (505 insns) */
#include "types.h"

typedef struct { s32 w[17]; } Blk68;

extern u8   class_w[];
extern Blk68 cw96_active;
extern Blk68 cw98_active;

extern s32 sub_sub_step_up();
extern s32 goto_sub_sub_step();
extern s32 palette_upload_enqueue();
extern s32 device_cmd_send();
extern s32 device_port_init();
extern s32 get_CD_State2();
extern s32 cd_read_start2();
extern s32 game_reset();
extern s32 don_init();
extern s32 tpage_buf_clear();
extern s32 class_clear_f();
extern s32 text_sys_init();
extern s32 set_kanji_string();
extern s32 k_disp_start();
extern s32 k_reset();
extern s32 check_end_k();
extern s32 indexed_byte_lsb_set();
extern s32 value_cache_find_or_load();
extern s32 dec_bg_init();
extern s32 bustup_cd_read();
extern s32 bustup_kuchi_ani();
extern s32 basyo_init();
extern s32 date_light_effect();
extern s32 bustup_ptr_init();
extern s32 fade_up_main();
extern s32 fade_down_main();
extern s32 fade_in();
extern s32 fade_out_step();
extern s32 fade_out();
extern s32 sprite_fade_main();
extern s32 menu2_main();
extern s32 bustup_face_change();
extern s32 zyotai_sprite_set_chk();
extern s32 bustup_ani();
extern s32 date_name_set();
extern s32 yazirushi_off();
extern s32 date_sprite_init();
extern s32 date_object_set();
extern s32 date_fade_in();
extern s32 date_fade_out();
extern s32 serifu_run();
extern s32 event_log_add();
extern s32 Vblnk_Timer_Init();
extern s32 Vblnk_Timer();
extern s32 common_disp();
extern s32 taiiku_bufptr_init3();
extern s32 taiiku_tex_ptrs_set();

extern u8  now_sub_sub_step;
extern s32 load_done_flag;
extern u8  taiiku_talk_res;
extern u8  taiiku_sub_sub_main_str0;
extern u8  taiiku_sub_sub_main_str1;
extern u8  taiiku_sub_sub_main_str2;
extern u8  now_girl;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern s32 taiiku_girl1_serifu;
extern s32 taiiku_girl1_serifu_1;
extern s32 taiiku_girl1_serifu_2;
extern s32 serifu_no;
extern volatile s16 girl1_yuukou;
extern volatile s16 girl1_tokimeki;
extern volatile s16 girl1_syoushin;
extern s16 mes_base_y;
extern s32 taiiku_girl1_dobj[];
extern s16 cw98_anim;
extern u8  cw98_disp;
extern u8  cw98_bright;
extern u8  cw98_anim_flag;
extern s16 cw98_frame;
extern volatile s16 cal_cw_anim;
extern s16 cw97_anim;
extern s16 cw97_frame;
extern s16 cw97_tick;
extern s32 tpage_buf[];

s32 taiiku_sub_sub_main(void) {
    s32 v0;
    s32 v1;
    s32 t;
    u8 *p;
    u8 *pend;

    bustup_ani();
    switch ((u32)now_sub_sub_step) {
    case 0:
        load_done_flag = 0;
        text_sys_init();
        tpage_buf_clear();
        tpage_buf[26] = -1;
        class_clear_f();
        value_cache_find_or_load(16700, 1);
        taiiku_talk_res = 0;
        sub_sub_step_up();
        break;
    case 1:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) break;
        dec_bg_init(1);
        bustup_cd_read(24413);
        sub_sub_step_up();
        break;
    case 2:
        sprite_fade_main();
        break;
    case 3:
        date_light_effect();
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) break;
        k_reset(0);
        basyo_init((s32)&taiiku_sub_sub_main_str0);
        k_disp_start(1);
        bustup_ptr_init();
        now_girl = 1;
        zyotai_sprite_set_chk();
        break;
    case 4:
        date_light_effect();
        cd_read_start2(1, 0x801E8000, 0xB7C5);
        sub_sub_step_up();
        break;
    case 5:
        date_light_effect();
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) break;
        yazirushi_off();
        date_name_set(1);
        taiiku_bufptr_init3();
        now_serifu = 0;
        now_serifu_in = 0;
        ss_serifu = taiiku_girl1_serifu;
        ss_serifu_kind = taiiku_girl1_serifu_1;
        sss_sound = taiiku_girl1_serifu_2;
        sub_sub_step_up();
        break;
    case 6:
    case 8:
    case 12:
    case 14:
        serifu_run();
        date_light_effect();
        break;
    case 7:
        fade_up_main();
        date_light_effect();
        break;
    case 9:
        menu2_main();
        date_light_effect();
        break;
    case 10:
        v1 = serifu_no;
        switch (v1) {
        case 0:
            goto_sub_sub_step(17);
            break;
        case 1:
            goto_sub_sub_step(11);
            break;
        }
        date_light_effect();
        break;
    case 11:
        girl1_yuukou = girl1_yuukou - 1;
        girl1_tokimeki = girl1_tokimeki - 1;
        girl1_syoushin = girl1_syoushin + 10;
        bustup_face_change(3);
        bustup_kuchi_ani();
        now_serifu = 10;
        now_serifu_in = 0;
        date_light_effect();
        sub_sub_step_up();
        break;
    case 13:
        date_light_effect();
        fade_down_main();
        break;
    case 15:
        fade_out_step();
        break;
    case 16:
        taiiku_talk_res = 2;
        break;
    case 17:
        serifu_run();
        date_light_effect();
        break;
    case 18:
        date_light_effect();
        value_cache_find_or_load(16506, 1);
        sub_sub_step_up();
        break;
    case 19:
        fade_out();
        break;
    case 20:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) break;
        k_reset(0);
        basyo_init((s32)&taiiku_sub_sub_main_str1);
        mes_base_y = 192;
        v0 = dec_bg_init(1);
        indexed_byte_lsb_set(1, v0);
        cd_read_start2(69, 0x80197000, 0xBB23);
        device_port_init(0, 513);
        sub_sub_step_up();
        break;
    case 21:
        fade_in();
        break;
    case 22:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) break;
        taiiku_tex_ptrs_set();
        palette_upload_enqueue(taiiku_girl1_dobj[4], 17, 1, 2, 0);
        date_sprite_init(taiiku_girl1_dobj[5], taiiku_girl1_dobj[6], taiiku_girl1_dobj[7], taiiku_girl1_dobj[8],
                      taiiku_girl1_dobj[9], taiiku_girl1_dobj[10]);
        date_object_set(taiiku_girl1_dobj[1], taiiku_girl1_dobj[2], taiiku_girl1_dobj[0], (s16)(*(s16 *)&taiiku_girl1_dobj[3]));
        pend = class_w + 136;
        p = class_w;
        do {
            v1 = p[6532];
            p += 68;
            p[6464] = (u8)(v1 | 8);
        } while (p != pend);
        cw98_active = cw96_active;
        cw98_anim = 4;
        sub_sub_step_up();
        break;
    case 23:
        serifu_run();
        break;
    case 24:
        don_init();
        k_reset(1);
        set_kanji_string(-128, 50, 0, (s32)&taiiku_sub_sub_main_str2, 0);
        device_cmd_send(1546);
        Vblnk_Timer_Init();
        sub_sub_step_up();
        break;
    case 25:
        v0 = Vblnk_Timer();
        if ((u32)v0 < 61) break;
        value_cache_find_or_load(18209, 1);
        k_reset(1);
        sub_sub_step_up();
        break;
    case 26:
        fade_out();
        break;
    case 27:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) break;
        v0 = dec_bg_init(1);
        indexed_byte_lsb_set(1, v0);
        sub_sub_step_up();
        break;
    case 28:
        date_fade_in();
        cw98_disp |= 0x80;
        t = (cw98_bright + 4) & 0xFF;
        cw98_bright = (u8)t;
        if ((u32)t < 128) break;
        cw98_bright = 128;
        break;
    case 29:
        serifu_run();
        v0 = check_end_k();
        if (v0 != 0) {
            cw98_anim_flag = 0;
            cw98_frame = 0;
        } else {
            cw98_anim_flag = 1;
        }
        break;
    case 30:
        serifu_run();
        break;
    case 31:
        cw98_anim = 5;
        sub_sub_step_up();
        break;
    case 32:
        serifu_run();
        v0 = check_end_k();
        if (v0 != 0) {
            cw98_anim_flag = 0;
            cw98_frame = 0;
        } else {
            cw98_anim_flag = 1;
        }
        break;
    case 33:
        cal_cw_anim = 3;
        cw97_anim = 1;
        cw97_frame = 0;
        cw97_tick = 0;
        sub_sub_step_up();
        break;
    case 34:
        serifu_run();
        break;
    case 35:
        serifu_run();
        v0 = check_end_k();
        if (v0 != 0) {
            cw98_anim_flag = 0;
            cw98_frame = 0;
        } else {
            cw98_anim_flag = 1;
        }
        break;
    case 36:
        serifu_run();
        break;
    case 37:
        date_fade_out();
        t = (cw98_bright - 4) & 0xFF;
        cw98_bright = (u8)t;
        if ((u32)t >= 3) break;
        cw98_bright = 0;
        break;
    case 38:
        taiiku_talk_res = 1;
        event_log_add(2, 13);
        girl1_yuukou = girl1_yuukou + 2;
        girl1_tokimeki = girl1_tokimeki + 1;
        break;
    default:
        game_reset();
        break;
    }
    common_disp();
    return taiiku_talk_res;
}
