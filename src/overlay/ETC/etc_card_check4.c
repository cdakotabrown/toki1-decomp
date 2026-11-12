/* @SymbolStart: 0x80141748 */
/* @RodataBase: 0x8014D084 */
/* Address: 0x80141750 */
/* Size: 540 bytes (135 insns) */
/* @SymbolSize: 548 */
#include "types.h"

extern void k_sub_reset(void);
extern void set_kanji_string(s32, s32, s32, s32, s32);
extern void k_sub_disp_start(s32);
extern void card_step_init(void);
extern s32  card_check_main(void);
extern void card_state_clear(void);
extern void printf(s32);
extern void goto_sub_sub_step(s32);
extern s32  card_file_scan_main(void);
extern void sub_sub_step_up(void);
extern void dec_bg_show(s32);

extern u8  step_mode;
extern u8  card_result;
extern u32 card_cnt;
extern u8  card_sys_present;
extern u8  etc_card_check4_str0[];
extern u8  etc_card_check4_str1[];
extern u8  etc_card_check4_str2[];
extern u8  etc_card_check4_str3[];
extern u8  etc_card_check4_str4[];

void etc_card_check4(void) {
    switch (step_mode) {
    case 0:
        k_sub_reset();
        set_kanji_string(-120, 48, 0, (s32)etc_card_check4_str0, 0);
        k_sub_disp_start(1);
        step_mode = step_mode + 1;
        card_step_init();
        break;
    case 1:
        if (card_check_main() != 0) {
            if (card_result == 1) {
                card_state_clear();
                printf((s32)etc_card_check4_str1);
                step_mode = step_mode + 1;
            } else if (card_result == 241) {
                card_state_clear();
                printf((s32)etc_card_check4_str2);
                goto_sub_sub_step(160);
            } else if (card_result == 255) {
                card_state_clear();
                printf((s32)etc_card_check4_str3);
                goto_sub_sub_step(255);
            }
            break;
        }
        if (card_cnt >= 513) {
            card_state_clear();
            printf((s32)etc_card_check4_str4);
            goto_sub_sub_step(255);
        }
        break;
    case 2:
        card_step_init();
        step_mode = step_mode + 1;
        break;
    case 3:
        if (card_file_scan_main() == 2) {
            step_mode = step_mode + 1;
            card_state_clear();
        } else if (card_cnt >= 513) {
            goto_sub_sub_step(255);
        }
        break;
    case 4:
        if (card_sys_present != 0) {
            sub_sub_step_up();
        } else {
            goto_sub_sub_step(240);
        }
        break;
    }
    dec_bg_show(2);
}
