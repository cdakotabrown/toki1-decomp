/* @RodataBase: 0x80154BD0 */
/* @PostProc: patch_stack_complete --old 88 --new 80 */
/* @PostProc: remap_stack_slots --from 76 --to 60 */
/* @PostProc: swap_instructions --offset1 0xec --offset2 0xf0 */
/* @PostProc: swap_instructions --offset1 0xf8 --offset2 0x104 */
/* @PostProc: swap_instructions --offset1 0x11c --offset2 0x138 */
/* @PostProc: swap_instructions --offset1 0x120 --offset2 0x138 */
/* @PostProc: swap_instructions --offset1 0x124 --offset2 0x138 */
/* @PostProc: swap_instructions --offset1 0x128 --offset2 0x138 */
/* @PostProc: swap_instructions --offset1 0x12c --offset2 0x134 */
/* @PostProc: swap_instructions --offset1 0x1b8 --offset2 0x1c0 */
/* @PostProc: swap_instructions --offset1 0x378 --offset2 0x37c */
/* @PostProc: swap_instructions --offset1 0x37c --offset2 0x380 */
/* @PostProc: swap_instructions --offset1 0x4d8 --offset2 0x4dc */
/* @PostProc: patch_insn_reg_field 0x28:rt:t1:t0 0x34:rt:t1:t0 0x40:rt:t2:t1 0x44:rs:t2:t1 0x44:rt:t2:t1 0x4c:rs:t2:t1 0x54:rt:t2:t1 0x54:rd:t2:t1 0x5c:rt:t2:t1 0x60:rt:t2:t1 0x68:rs:t2:t1 0x78:rt:t3:t2 0x7c:rt:t4:t3 0x80:rt:t4:t3 0x84:rt:t3:t2 0xa8:rt:t5:t4 0xac:rs:t5:t4 0xac:rt:t5:t4 0xb0:rt:t6:t8 0xb4:rs:t6:t8 0xb4:rt:t6:t8 0xb8:rt:t5:t4 0xb8:rd:t9:t5 0xbc:rt:t9:t5 0xbc:rd:t8:t9 0xc4:rt:t2:t0 0xc8:rs:t2:t0 0xc8:rt:t2:t0 0xcc:rt:t8:t9 0xd0:rt:t6:t8 0xd0:rd:t7:t6 0xd4:rt:t7:t6 0xd4:rd:t1:t7 0xdc:rt:t1:t7 0xe0:rt:t2:t0 0xe0:rd:t3:t1 0xe4:rt:t3:t1 0xe4:rd:t4:t2 0xf4:rt:t0:a0 0xf8:rs:t0:a0 0xf8:rt:t0:a0 0x104:rt:t4:t2 0x108:rt:a0:t3 0x10c:rt:a1:t9 0x110:rt:a2:t7 0x114:rt:a3:t2 0x118:rt:a0:t3 0x118:rd:t2:t4 0x11c:rt:t2:t4 0x11c:rd:t3:t5 0x120:rt:a1:t9 0x120:rd:t5:t8 0x124:rt:a2:t7 0x124:rd:t8:t0 0x128:rt:a3:t2 0x128:rd:t7:t3 0x12c:rt:t7:t3 0x12c:rd:t1:t4 0x130:rt:t8:t0 0x130:rd:t6:t1 0x134:rt:t5:t8 0x134:rd:t9:t6 0x13c:rt:t9:t6 0x140:rt:t6:t1 0x144:rt:t1:t4 0x14c:rt:t0:a0 0x150:rt:t3:t5 0x1a8:rt:t4:t5 0x1b0:rt:t4:t5 0x1b4:rt:t5:at 0x1bc:rt:at:t9 0x1c0:rs:t5:t9 0x1c0:rt:t5:t9 0x1c8:rs:t5:t9 0x1c8:rt:t9:t8 0x1cc:rt:t9:t8 0x1e8:rt:t8:t6 0x1ec:rs:t8:t6 0x1ec:rt:t8:t6 0x1f4:rs:t8:t6 0x1f4:rt:t6:t7 0x1fc:rt:t6:t7 0x228:rt:t7:t0 0x22c:rs:t7:t0 0x22c:rt:t7:t0 0x234:rs:t7:t0 0x32c:rt:t1:t0 0x334:rt:t1:t0 0x360:rt:v1:v0 0x364:rs:v1:v0 0x364:rt:v1:v0 0x368:rt:v0:v1 0x36c:rs:v0:v1 0x36c:rt:v0:v1 0x370:rs:v1:v0 0x370:rt:t2:t1 0x374:rs:t2:v1 0x374:rt:v0:t1 0x378:rt:t5:t2 0x37c:rs:t5:t2 0x37c:rt:t5:t2 0x380:rt:v1:v0 0x388:rs:t4:t2 0x388:rt:t5:t4 0x38c:rt:t9:t5 0x390:rs:t9:t5 0x390:rt:t9:t5 0x394:rt:t6:t8 0x398:rs:t9:t5 0x398:rt:t8:t9 0x39c:rs:t8:t9 0x3a4:rt:t6:t8 0x3bc:rt:v1:v0 0x3c0:rs:v1:v0 0x3c0:rt:v1:v0 0x3c4:rt:t3:t7 0x3c8:rs:v1:v0 0x3c8:rt:t7:t6 0x3cc:rs:t7:v1 0x3cc:rt:v0:t6 0x3d0:rt:v1:v0 0x3d0:rd:t1:t0 0x3d4:rs:t3:t7 0x3d4:rt:t3:t7 0x3d8:rt:t1:t0 0x3d8:rd:t2:t1 0x3dc:rs:t2:t7 0x3dc:rt:t3:t1 0x3e0:rt:t4:t3 0x3e4:rs:t4:t3 0x3e4:rt:t4:t3 0x3ec:rs:t4:t3 0x3ec:rt:t5:t2 0x3f0:rs:t5:t2 0x430:rt:t9:t4 0x438:rt:t9:t4 0x450:rt:t8:t5 0x454:rs:t8:t5 0x454:rt:t8:t5 0x45c:rs:t8:t5 0x548:rt:t6:t9 0x54c:rs:t6:t9 0x54c:rt:t6:t9 0x554:rs:t6:t9 */
/* Address: 0x80144F94 */
/* Size: 1380 bytes (345 insns) */
/* @SymbolSize: 1380 */
/* @CompileFlags: -Wo,-loopunroll,0 */
#include "types.h"

typedef struct { u8 b[11]; } DateStamp;

extern void sub_step_up(void);
extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void k_reset(s32);
extern void set_dec_bri(s32);
extern void dec_bg_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern void parameter_disp_switch(s32);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void icon_disp_switch(s32);
extern void basyo_init(s32);
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
extern s32 get_g_zyotai_low(s32);
extern void bunkaken_common_bank_set(void);
extern void bunkaken_state_up(void);
extern void bunkaken_girl_para_add(void);
extern void bunkaken_event_log_add(void);
extern void bunkaken_sub_dispatch(void);
extern void bunkaken_birth_main(void);
extern void bunkaken_statea(void);

extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern u8 girl_appear[];
extern u8  bunkaken_main_str0[];
extern u8  now_month;
extern u8  now_day;
extern u8  girl_blk[];
extern u8  girl_blk_end[];
extern s32 girl0_flag;
extern s32 girl0_birth;
extern s32 girl1_flag;
extern s32 girl2_flag;
extern s32 p_birthday;
extern u8  day_flag;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern s32 work_miura[];
extern s32 work_takak[];
extern s32 common_disp_off;
extern s32 bunka_girl2;
extern s32 bunka_girl;
extern s32 bunka_state;
extern DateStamp bunkaken_main_tbl;
extern s32 bunkaken_bank_ptr[];

void bunkaken_main(void) {
    DateStamp stamp;
    s32 v0;
    s32 n;
    s32 *wsrc;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    u32 e0;
    u8 *dst;
    s32 *p;

    stamp = bunkaken_main_tbl;
    bustup_ani();

    switch (bunka_state) {
    case 0:
        device_cmd_send(191);
        device_load_start(1, 0xC486, 0xC459, 0xD266, 0xD216, 0xD20E);
        date_x_taku_set(1, 0);
        girl_appear[0] = (u32)(girl0_flag << 30) >> 31;
        girl_appear[1] = (u32)(girl1_flag << 30) >> 31;
        girl_appear[2] = (u32)(girl2_flag << 30) >> 31;
        wsrc = (s32 *)girl_blk;
        dst = (&girl_appear[3]);
        do {
            a = wsrc[114];
            b = wsrc[128];
            c = wsrc[142];
            d = wsrc[156];
            wsrc += 56;
            e0 = (u32)(a << 30) >> 31;
            dst[1] = (u32)(b << 30) >> 31;
            dst[2] = (u32)(c << 30) >> 31;
            dst[3] = (u32)(d << 30) >> 31;
            dst += 4;
            dst[-4] = e0;
        } while (wsrc != (s32 *)girl_blk_end);
        shared_system_state_reset();
        dec_bg_reset();
        message_window_init();
        k_reset(0);
        basyo_init(bunkaken_main_str0);
        mes_window_switch(0);
        mascot_disp_switch(0);
        hizuke_init();
        icon_disp_switch(0);
        parameter_disp_switch(0);
        bunka_girl2 = 255;
        common_disp_off = 0;
        bunka_state = bunka_state + 1;
        bunka_girl = 255;
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
        bunkaken_state_up();
        break;
    case 5:
        if (get_CD_State2() & 1) {
            yazirushi_off();
            date_name_set(0);
            bunkaken_common_bank_set();
            now_serifu = 0;
            now_serifu_in = 0;
            ss_serifu = bunkaken_bank_ptr[0];
            ss_serifu_kind = bunkaken_bank_ptr[1];
            sss_sound = bunkaken_bank_ptr[2];
            bunka_state = bunka_state + 1;
        }
        break;
    case 6:
        serifu_run();
        bunkaken_state_up();
        break;
    case 7:
        bunkaken_sub_dispatch();
        break;
    case 8:
        v0 = girl0_birth;
        if (now_month == (v0 & 0xF) &&
            now_day == ((u32)(v0 << 23) >> 27) &&
            (day_flag & 1) == 0) {
            bunka_state = 9;
            now_sub_step = 0;
            now_sub_sub_step = 0;
            break;
        }
        v0 = p_birthday;
        if (now_month == (v0 & 0xF) &&
            now_day == ((u32)(v0 << 23) >> 27) &&
            (day_flag & 4) == 0) {
            if ((u32)get_g_zyotai_low(bunka_girl) < 2) goto Lset10;
            if (bunka_girl == 11) {
                if ((u32)get_g_zyotai_low(0) < 2) {
Lset10:
                    bunka_state = 10;
                    now_sub_step = 0;
                    now_sub_sub_step = 0;
                    break;
                }
            }
        }
        if (now_sub_step == 0) {
            device_cmd_send(191);
            season_bgm_set();
            sub_step_up();
            break;
        }
        if (device_load_poll() != 1) {
            break;
        }
        bunkaken_girl_para_add();
        if (bunka_girl != 255) {
            if ((u32)get_g_zyotai_low(bunka_girl = bunka_girl) < 4) {
                bunkaken_event_log_add();
            }
        }
        p = work_miura;
        do {
            p += 4;
            p[-3] = 0;
            p[-2] = 0;
            p[-1] = 0;
            p[-4] = 0;
        } while (p != work_takak);
        shared_system_state_reset();
        dec_bg_reset();
        set_dec_bri(0);
        return_schedule(1);
        break;
    case 9:
        bunkaken_birth_main();
        break;
    case 10:
        bunkaken_statea();
        break;
    default:
        game_reset();
        break;
    }

    if (common_disp_off == 0) {
        common_disp();
    }
}
