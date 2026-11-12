/* @CompileFlags: -Wc,-pic0 */
/* @SymbolStart: 0x8013F384 */
/* @SymbolSize: 576 */
/* @RodataBase: 0x8014C220 */
/* @PostProc: remap_stack_slots --from 52 --to 48 */
/* name_ent_loadc2 - NAME_ENT overlay, Address: 0x8013F38C, 142 insns. */
#include "types.h"

extern void card_step_init(void);
extern s32  card_file_read_main2(void);
extern void save_flag_clear(void);
extern void card_state_clear(void);
extern s32  save_check(s32);
extern s32  save_game_restore(s32);
extern void name_ent_album_disp_init(void);
extern void name_ent_card_io_noop(void);
extern void goto_sub_step(s32);
extern void text_sys_init(void);
extern void set_kanji_string(s32, s32, s32, u8 *, s32);
extern void k_disp_start(s32);
extern void sub_sub_step_up(void);
extern void strcpy(u8 *, u8 *);
extern void strcat(u8 *, u8 *);
extern void printf(u8 *);

extern u8  step_mode;
extern u8  card_result;
extern s32 card_cnt;
extern u8  now_album;
extern u8  album_status[];
extern u32 album_info_disp_tbl0[];
extern u8  name_ent_copy_sum_fmt[];
extern u8  name_ent_copy_src_str[];
extern u8  name_ent_copy_ask_str[];
extern u8  name_ent_loadc2_str0[];
extern u8 step_mode;

void name_ent_loadc2(void)
{
    u8 textBuf[260];
    u8 idx;
    s32 v;
    s32 t;

    switch (step_mode) {
    case 0:
        card_step_init();
        step_mode++;
        break;
    case 1:
        if (card_file_read_main2() == 4) {
            step_mode++;
        } else {
            v = card_result;
            if (v == 240) {
                save_flag_clear();
                goto_sub_step(1);
            } else {
                t = card_cnt;
                if ((u32)t >= 513 || v == 208) {
                    card_state_clear();
                    goto_sub_step(255);
                }
            }
        }
        break;
    case 2:
        if (save_check(0) == 1) {
            name_ent_album_disp_init();
            step_mode++;
        } else {
            album_status[now_album] = 255;
            goto_sub_step(255);
        }
        break;
    case 3:
        if (save_game_restore(0) == 1) {
            name_ent_card_io_noop();
            printf(name_ent_copy_sum_fmt);
            step_mode++;
        } else {
            album_status[now_album] = 255;
            goto_sub_step(255);
        }
        break;
    case 4:
        text_sys_init();
        idx = now_album;
        strcpy(textBuf, (u8 *)album_info_disp_tbl0[idx]);
        strcat(textBuf, name_ent_copy_src_str);
        set_kanji_string(-64, 32, 15, textBuf, 0);
        set_kanji_string(-64, 48, 15, name_ent_copy_ask_str, 0);
        set_kanji_string(-64, 64, 15, name_ent_loadc2_str0, 0);
        k_disp_start(3);
        sub_sub_step_up();
        break;
    }
}
