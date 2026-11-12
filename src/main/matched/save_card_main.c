/* Function: save_card_main */
/* Address: 0x8005C104 */
/* Size: 704 bytes (176 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_insn_reg_field --patch 0x44:rt:v1:v0 --patch 0x48:rs:v1:v0 --patch 0x278:rt:v1:v0 --patch 0x288:rt:v1:v0 --patch 0x288:rs:v1:v0 --patch 0x290:rs:v1:v0 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 step_mode;
extern u8 card_result;
extern s32 card_cnt;
extern u8 card_sys_present;

extern char save_card_main_str0[];
extern char save_card_main_str1[];

extern s32 card_check_main(void);
extern void card_state_clear(void);
extern void card_step_init(void);
extern s32 card_file_scan_main(void);
extern void goto_sub_sub_step(s32 a0);
extern s32 card_file_read_main(void);
extern void save_work_restore(void);
extern void cursor_work_copy(s32 a0);
extern void k_sub_reset(void);
extern void set_kanji_string(s32, s32, s32, char *, s32);
extern void k_disp_switch(s32, s32);
extern void k_disp_start(s32 a0);
extern void sub_sub_step_up(void);
extern void k_disp_inc(void);

void save_card_main(void) {
    s32 result;
    u8 subState;
    u8 state;

    state = step_mode;

    switch (state) {
    case 0:
        result = card_check_main();
        if (result != 0) {
            subState = card_result;
            if (subState == 1) {
                card_state_clear();
                step_mode++;
            } else if (subState == 241) {
                card_state_clear();
                goto_sub_sub_step(160);
            } else if (subState == 255) {
                card_state_clear();
                goto_sub_sub_step(255);
            }
        } else {
            if ((u32)card_cnt >= 513u) {
                card_state_clear();
                goto_sub_sub_step(255);
            }
        }
        break;

    case 1:
        card_step_init();
        step_mode++;
        break;

    case 2:
        result = card_file_scan_main();
        if (result == 2) {
            step_mode++;
        } else {
            if ((u32)card_cnt >= 513u) {
                goto_sub_sub_step(255);
            }
        }
        break;

    case 3:
        if (card_sys_present != 0) {
            card_state_clear();
            card_step_init();
            step_mode++;
        } else {
            card_state_clear();
            k_sub_reset();
            set_kanji_string(-128, 50, 0, save_card_main_str0, 0);
            k_disp_switch(1, 1);
            k_disp_switch(2, 1);
            k_disp_start(4);
            sub_sub_step_up();
        }
        break;

    case 4:
        result = card_file_read_main();
        if (result == 3) {
            save_work_restore();
            cursor_work_copy(1);
            k_sub_reset();
            set_kanji_string(-128, 50, 0, save_card_main_str1, 0);
            k_disp_switch(1, 1);
            k_disp_switch(2, 1);
            k_disp_start(4);
            sub_sub_step_up();
            card_state_clear();
        } else {
            subState = card_result;
            if ((u32)card_cnt >= 513u || subState == 240) {
                card_state_clear();
                goto_sub_sub_step(255);
            }
        }
        break;
    }

    k_disp_inc();
}
