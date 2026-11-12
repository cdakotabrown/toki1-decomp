/* Function: data_load_confirm */
/* Address: 0x8005D7AC */
/* Size: 664 bytes (166 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void k_sub_reset(void);
extern void set_kanji_string(s16 x, s16 y, s32 a2, u8 *a3, s32 stack0);
extern void k_sub_disp_start(s32 a0);
extern s32 card_file_read_main2(void);
extern void card_state_clear(void);
extern void goto_sub_sub_step(s32 a0);
extern s32 save_check(s32 a0);
extern void album_info_disp(s32 a0, s32 a1);
extern s32 get_pad_str(void);
extern s32 get_pad_str2(void);
extern void device_cmd_send(s32 a0);
extern void sub_sub_step_up(void);
extern void slot_box_disp(void);
extern s32 save_game_restore(s32 a0);

extern u8 step_mode;
extern u32 card_cnt;
extern u8 card_result;
extern s32 pad_trig;
extern u8 data_load_confirm_str0;
extern u8 data_load_confirm_str1;
extern u8 data_load_confirm_str2;
extern u8 save_format_main_str6;
extern u8 save_format_main_str7;

void data_load_confirm(void) {
    s32 result;

    switch (step_mode) {
        case 0:
            k_sub_reset();
            set_kanji_string(-128, 50, 2, &data_load_confirm_str0, 0);
            set_kanji_string(-128, 66, 2, &data_load_confirm_str1, 0);
            k_sub_disp_start(2);
            step_mode++;
            goto done;

        case 1:
            result = card_file_read_main2();
            if (result == 4) {
                step_mode++;
                card_state_clear();
                goto done;
            }
            if (card_cnt < 513) {
                if (card_result != 240) {
                    goto done;
                }
            }
            card_state_clear();
            goto_sub_sub_step(255);
            goto done;

        case 2:
            result = save_check(0);
            if (result == 1) {
                step_mode++;
                goto done;
            }
            goto_sub_sub_step(224);
            goto done;

        case 3:
            k_sub_reset();
            album_info_disp(0, 1);
            set_kanji_string(-128, 50, 1, &data_load_confirm_str2, 0);
            set_kanji_string(-128, 66, 0, &save_format_main_str6, 0);
            result = get_pad_str();
            set_kanji_string(-44, 66, 1, (u8 *)result, 0);
            set_kanji_string(-128, 82, 0, &save_format_main_str7, 0);
            result = get_pad_str2();
            set_kanji_string(-44, 82, 2, (u8 *)result, 0);
            k_sub_disp_start(11);
            step_mode++;
            goto done;

        case 4:
            if (pad_trig & 0x20) {
                device_cmd_send(1281);
                step_mode++;
                goto done;
            }
            if (pad_trig & 0x40) {
                goto_sub_sub_step(6);
            }
            goto done;

        case 5:
            result = save_game_restore(0);
            if (result == 1) {
                sub_sub_step_up();
                goto done;
            }
            goto_sub_sub_step(224);
            goto done;

        default:
            goto done;
    }

done:
    slot_box_disp();
}
