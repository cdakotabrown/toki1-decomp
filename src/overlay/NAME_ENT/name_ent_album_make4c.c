/* @RodataBase: 0x8014C1A8 */
/* @SymbolStart: 0x8013E114 */
/* @SymbolSize: 884 */
/* @CompileEnv: IDO52_NO_DEADSTORE_GLOBALS=0x0 */
/* @PostProc: patch_stack_complete --old 304 --new 320 */
/* @PostProc: remap_stack_slots --from 48 --to 52 */
/* @PostProc: swap_instructions --offset1 0x19c --offset2 0x1a0 */
/* @PostProc: swap_instructions --offset1 0x1a0 --offset2 0x1a4 */
/* Address: 0x8013E114 (absorbed head) / 0x8013E11C (prologue) */
/* Size: 884 bytes (221 insns) */
#include "types.h"

extern void text_sys_init(void);
extern void strcpy(void *buf, s32 value);
extern void strcat(void *buf, u8 *str);
extern void set_kanji_string(s32 x, s32 y, s32 w, void *src, s32 flag);
extern void k_disp_start(s32 mode);
extern s32  card_file_make(s32 arg);
extern void goto_sub_step(s32 arg);
extern void card_state_clear(void);
extern void card_step_init(void);
extern void save_state_set(void);
extern s32  card_file_write_main(void);
extern void save_flag_clear(void);
extern void goto_step(s32 arg);
extern s32  card_slot_write_main(void);
extern void _game_work_init_set(void);
extern void game_work_init2(void);
extern void printf(u8 *str);
extern void name_ent_option_restore(void);

extern u8   now_album;
extern u8   step_mode;
extern u8   card_result;
extern s32  card_cnt;
extern u8   last_album;
extern u8   album_status[15];
extern s32 *card_file_save_tbl0;
extern u8   name_ent_album_make4c_str0[];
extern u8   name_ent_album_make4c_str1[];
extern u8   name_ent_album_make4c_str2[];
extern u8   name_ent_fileno_fmt[];

void name_ent_album_make4c(void)
{
    u8 buf[256];
    s32 i;
    u32 step;

    step = step_mode;

    switch (step) {
    case 0:
        text_sys_init();
        strcpy(buf, card_file_save_tbl0[now_album - 1]);
        strcat(buf, name_ent_album_make4c_str0);
        set_kanji_string(-64, 32, 15, buf, 0);
        set_kanji_string(-64, 48, 15, name_ent_album_make4c_str1, 0);
        set_kanji_string(-64, 64, 15, name_ent_album_make4c_str2, 0);
        k_disp_start(3);
        step_mode++;
        break;

    case 1:
        if (card_file_make(now_album) == 1) {
            step_mode++;
            break;
        }
        goto_sub_step(4);
        return;

    case 2:
        if (card_file_make(0) == 1) {
            step_mode++;
            break;
        }
        goto_sub_step(4);
        return;

    case 3:
        card_state_clear();
        last_album = now_album;
        album_status[0] = 0;
        album_status[1] = 0;
        album_status[2] = 0;
        i = -1;
        do {
            i += 4;
            album_status[i] = 0;
            album_status[i + 1] = 0;
            album_status[i + 2] = 0;
            album_status[i + 3] = 0;
        } while (i != 11);
        album_status[now_album] = 1;
        album_status[0] = 1;
        now_album = 0;
        card_step_init();
        save_state_set();
        step_mode++;
        break;

    case 4:
        if (card_file_write_main() == 19) {
            card_state_clear();
            step_mode++;
            break;
        }
        if (card_cnt < 513U && card_result != 240) {
            return;
        }
        card_state_clear();
        save_flag_clear();
        goto_sub_step(1);
        return;

    case 5:
        card_step_init();
        _game_work_init_set();
        game_work_init2();
        game_work_init2();
        now_album = last_album;
        printf(name_ent_fileno_fmt);
        step_mode++;
        break;

    case 6:
        if (card_slot_write_main() == 20) {
            album_status[now_album] = 1;
            step_mode++;
            break;
        }
        if (card_cnt < 513U && card_result != 240) {
            return;
        }
        card_state_clear();
        save_flag_clear();
        goto_sub_step(1);
        return;

    case 7:
        card_state_clear();
        name_ent_option_restore();
        goto_step(20);
        break;
    }
}
