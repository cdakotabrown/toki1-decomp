/* @SymbolStart: 0x8014470C */
/* @RodataBase: 0x8014D424 */
/* Address: 0x80144714 */
/* Size: 688 bytes (170 instructions) */
/* @SymbolSize: 696 */
/* @PostProc: patch_insn_reg_field 0x280:rt:t9:v0 0x290:rs:t9:v0 0x290:rt:t9:v0 0x298:rs:t9:v0 */
#include "types.h"

extern s32  card_check_main(void);
extern void card_state_clear(void);
extern void printf(s32);
extern void goto_sub_sub_step(s32);
extern void card_step_init(void);
extern s32  card_file_scan_main(void);
extern s32  card_file_read_main(void);
extern void save_work_restore(void);
extern void text_sys_init(void);
extern void set_kanji_string(s32, s32, s32, s32, s32);
extern void k_disp_start(s32);
extern void sub_sub_step_up(void);
extern void k_disp_inc(void);

extern u8  card_result;
extern u8  step_mode;
extern s32 card_cnt;
extern u8  card_sys_present;
extern u8  save_file_card_check_str0[];
extern u8  save_file_card_check_str1[];
extern u8  save_file_card_check_str2[];
extern u8  save_file_card_check_str3[];
extern u8  save_file_card_check_str4[];
extern u8  save_file_card_check_str5[];

void save_file_card_check(void)
{
    s32 v0;

    switch (step_mode) {
    case 0:
        if (card_check_main() != 0) {
            v0 = card_result;
            if (v0 == 1) {
                card_state_clear();
                printf((s32)save_file_card_check_str0);
                step_mode++;
                break;
            }
            if (v0 == 241) {
                card_state_clear();
                printf((s32)save_file_card_check_str1);
                goto_sub_sub_step(160);
                break;
            }
            if (v0 == 255) {
                card_state_clear();
                printf((s32)save_file_card_check_str2);
                goto_sub_sub_step(255);
            }
            break;
        }
        if ((u32)card_cnt < 513) {
            break;
        }
        card_state_clear();
        printf((s32)save_file_card_check_str3);
        goto_sub_sub_step(255);
        break;
    case 1:
        card_step_init();
        step_mode++;
        break;
    case 2:
        if (card_file_scan_main() == 2) {
            step_mode++;
            break;
        }
        if ((u32)card_cnt < 513) {
            break;
        }
        goto_sub_sub_step(255);
        break;
    case 3:
        if (card_sys_present != 0) {
            card_state_clear();
            card_step_init();
            step_mode++;
            break;
        }
        card_state_clear();
        text_sys_init();
        set_kanji_string(-128, 50, 0, (s32)save_file_card_check_str4, 0);
        k_disp_start(1);
        sub_sub_step_up();
        break;
    case 4:
        if (card_file_read_main() == 3) {
            save_work_restore();
            text_sys_init();
            set_kanji_string(-128, 50, 0, (s32)save_file_card_check_str5, 0);
            k_disp_start(1);
            sub_sub_step_up();
            card_state_clear();
            break;
        }
        if ((u32)card_cnt < 513) {
            if (card_result != 240) {
                break;
            }
        }
        card_state_clear();
        goto_sub_sub_step(255);
        break;
    }
    k_disp_inc();
}
