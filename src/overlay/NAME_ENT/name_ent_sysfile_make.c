/* @RodataBase: 0x8014C434 */
/* @SymbolStart: 0x80141810 */
/* @SymbolSize: 1916 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=86:a0 IDO52_AS1_TIE_REJECT=226,227,228 IDO52_AS1_XBB_SKIP_CAND=7 IDO52_CMOT_KILL_AVAIL=47:60 */
/* @PostProc: patch_insn_reg_field 0x458:rs:t6:a2 0x458:rt:a2:t6 */
/* 2026-09-16: head symbol func_80141810 (0x80141810, `lbu t1,step_mode` + `la v1,album_work`) merged into this window (@SymbolStart 0x80141810, 1916 bytes); the former sel/p params are those two reads. Probe under a temporary non-ASM-ONLY marker: 9/479 words, 3 words over-long -- still the same blocker class (register-base and store-order divergences); the caller_threaded_selector home copy the blocker entry names is gone with the param, the rest of the wall is not. */
#include "types.h"

extern u8 step_mode;
extern u8 card_result;
extern u8 now_album;
extern u8 last_album;
extern u8 card_sys_present;
extern s32 pad_trig;
extern u32 step_counter;
extern u32 card_cnt;
extern u8 album_work[];
extern u8 name_ent_bukatu_cnt[];
extern u8 screen_w[];

extern u8 name_ent_sysfile_make_str0[];
extern u8 name_ent_sysfile_make_str1[];
extern u8 name_ent_sysfile_make_str2[];
extern u8 name_ent_sysfile_make_str3[];
extern u8 name_ent_sysfile_make_str4[];
extern u8 name_ent_sysfile_make_str5[];
extern u8 name_ent_sysfile_make_str6[];
extern u8 name_ent_sysfile_make_str7[];
extern u8 save_format_main_str6[];
extern u8 save_format_main_str7[];

extern void text_sys_init();
extern void set_kanji_string();
extern s32 get_pad_str();
extern s32 get_pad_str2();
extern void k_disp_start();
extern void save_flag_clear();
extern void goto_sub_step();
extern s32 card_file_make();
extern s32 card_file_read_main2();
extern void card_state_clear();
extern s32 save_check();
extern s32 save_game_restore();
extern void save_state_set();
extern void card_step_init();
extern s32 card_file_write_main();
extern void goto_sub_sub_step();
extern void device_cmd_send();

void name_ent_sysfile_make()
{
    s32 i;
    u8 *p;
    u8 *base;

    p = album_work;
    base = screen_w;
    switch (step_mode) {
    case 0:
        text_sys_init();
        set_kanji_string(-64, 32, 15, name_ent_sysfile_make_str0, 0);
        set_kanji_string(-64, 48, 15, name_ent_sysfile_make_str1, 0);
        set_kanji_string(-64, 64, 15, save_format_main_str6, 0);
        set_kanji_string(20, 64, 1, get_pad_str(), 0);
        set_kanji_string(-64, 80, 15, save_format_main_str7, 0);
        set_kanji_string(20, 80, 2, get_pad_str2(), 0);
        k_disp_start(6);
        step_mode++;
        break;
    case 1:
        if (pad_trig & 0x20) {
            step_mode++;
        } else if (pad_trig & 0x40) {
            save_flag_clear();
            goto_sub_step(1);
        }
        break;
    case 2:
        text_sys_init();
        set_kanji_string(-64, 32, 15, name_ent_sysfile_make_str2, 0);
        set_kanji_string(-64, 48, 15, name_ent_sysfile_make_str3, 0);
        set_kanji_string(-64, 64, 15, name_ent_sysfile_make_str4, 0);
        k_disp_start(3);
        step_counter = 0;
        step_mode++;
        break;
    case 3:
        if (pad_trig != 0 || step_counter >= 65) {
            step_mode++;
        }
        break;
    case 4:
        text_sys_init();
        set_kanji_string(-64, 32, 15, name_ent_sysfile_make_str5, 0);
        set_kanji_string(-64, 48, 15, name_ent_sysfile_make_str6, 0);
        k_disp_start(2);
        step_mode++;
        break;
    case 5:
        if (pad_trig != 0) {
            step_mode++;
        }
        break;
    case 6:
        text_sys_init();
        set_kanji_string(-64, 32, 15, name_ent_sysfile_make_str7, 0);
        k_disp_start(1);
        for (i = 0; i < 16; i++) {
            name_ent_bukatu_cnt[i] = 0;
        }
        now_album = 1;
        step_mode++;
        break;
    case 7:
        if (card_file_make(0) == 1) {
            card_sys_present = 1;
            step_mode++;
        } else {
            goto_sub_step(4);
        }
        break;
    case 8:
        if (now_album >= 15) {
            step_mode = 32;
            for (i = 0; i < 16; i++) {
                album_work[i + 20] = name_ent_bukatu_cnt[i];
            }
            break;
        }
        if ((p + 4)[now_album] == 0) {
            do {
                now_album++;
                if (now_album >= 15) {
                    for (i = 0; i < 16; i++) {
                        album_work[i + 20] = name_ent_bukatu_cnt[i];
                    }
                    step_mode = 32;
                }
            } while ((p + 4)[base[4376]] == 0);
        }
        if (step_mode != 32) {
            last_album = now_album;
            step_mode++;
            card_step_init();
        }
        break;
    case 9:
        if (card_file_read_main2() == 4) {
            card_state_clear();
            step_mode++;
        } else if (card_result == 240) {
            save_flag_clear();
            goto_sub_step(1);
        } else if (card_cnt >= 513 || card_result == 208) {
            card_state_clear();
            goto_sub_step(255);
        }
        break;
    case 10:
        if (save_check(0) == 1) {
            step_mode++;
        } else {
            p[now_album + 4] = 255;
            now_album++;
            step_mode = 8;
        }
        break;
    case 11:
        if (save_game_restore(0) == 1) {
            save_state_set();
            now_album++;
            step_mode = 8;
            for (i = 0; i < 16; i++) {
                if (screen_w[i + 1340] != 0) {
                    name_ent_bukatu_cnt[i]++;
                }
            }
        } else {
            p[now_album + 4] = 255;
            now_album++;
            step_mode = 8;
        }
        break;
    case 32:
        card_step_init();
        last_album = 0;
        step_mode++;
        break;
    case 33:
        if (card_file_write_main() == 19) {
            card_state_clear();
            goto_sub_step(2);
            goto_sub_sub_step(0);
            step_mode = 2;
            device_cmd_send(1295);
        } else if (card_cnt >= 513 || card_result == 240) {
            card_state_clear();
            save_flag_clear();
            goto_sub_step(1);
        }
        break;
    }
}
