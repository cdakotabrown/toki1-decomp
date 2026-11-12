/* @SymbolStart: 0x8013ED68 */
/* @RodataBase: 0x8014C1F0 */
/* @PostProc: patch_stack_complete --old 304 --new 312 */
/* @PostProc: remap_stack_slots --from 44 --to 48 */
/* @SymbolSize: 656 */
/* name_ent_album_makea8 - NAME_ENT overlay, Address: 0x8013ED70, 162 insns. */
#include "types.h"

extern u8  now_album;
extern u8  step_mode;
extern s32 card_cnt;
extern u8  card_result;
extern u8  last_album;
extern u32 *card_file_save_tbl0;
extern u8  album_status[];
extern u8  name_ent_album_makea8_str0[];
extern u8  name_ent_album_makea8_str1[];

extern void text_sys_init(void);
extern void strcpy(u8 *, u8 *);
extern void strcat(u8 *, u8 *);
extern void set_kanji_string(s32, s32, s32, u8 *, s32);
extern void k_disp_start(s32);
extern s32  card_file_make(s32);
extern void goto_sub_step(s32);
extern void card_step_init(void);
extern void _game_work_init_set(void);
extern void game_work_init2(void);
extern s32  card_slot_write_main(void);
extern void card_state_clear(void);
extern void save_flag_clear(void);
extern void save_state_set(void);
extern s32  card_file_write_main(void);
extern void name_ent_option_restore(void);
extern void goto_step(s32);
extern u8 step_mode;

void name_ent_album_makea8(void)
{
    u8 textBuf[260];
    u8 idx;
    s32 t;

    switch (step_mode) {
    case 0:
        text_sys_init();
        idx = now_album;
        strcpy(textBuf, (u8 *)card_file_save_tbl0[idx - 1]);
        strcat(textBuf, name_ent_album_makea8_str0);
        set_kanji_string(-64, 32, 15, textBuf, 0);
        set_kanji_string(-64, 48, 15, name_ent_album_makea8_str1, 0);
        k_disp_start(2);
        step_mode++;
        break;
    case 1:
        if (card_file_make(now_album) == 1) {
            step_mode++;
        } else {
            goto_sub_step(4);
        }
        break;
    case 2:
        card_step_init();
        _game_work_init_set();
        game_work_init2();
        step_mode++;
        break;
    case 3:
        if (card_slot_write_main() == 20) {
            album_status[now_album] = 1;
            step_mode++;
        } else {
            t = card_cnt;
            if ((u32)t >= 513 || card_result == 240) {
                card_state_clear();
                save_flag_clear();
                goto_sub_step(1);
            }
        }
        break;
    case 4:
        card_state_clear();
        card_step_init();
        last_album = now_album;
        save_state_set();
        step_mode++;
        break;
    case 5:
        if (card_file_write_main() == 19) {
            card_state_clear();
            step_mode++;
        } else {
            t = card_cnt;
            if ((u32)t >= 513 || card_result == 240) {
                card_state_clear();
                save_flag_clear();
                goto_sub_step(1);
            }
        }
        break;
    case 6:
        name_ent_option_restore();
        goto_step(20);
        break;
    }
}
