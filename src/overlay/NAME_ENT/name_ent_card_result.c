/* @PostProc: swap_registers v0:v1 */
/* @PostProc: patch_insn_reg_field 0x20:rs:v1:v0 */
/* name_ent_card_result - NAME_ENT overlay, Address: 0x8013C70C, Size: 228 bytes (57 insns) */
#include "types.h"

extern s32 card_check_main();
extern void set_flag_b3_high_bit();
extern void card_state_clear();
extern void printf();
extern void sub_step_up();
extern void goto_sub_step();
extern void sub_sub_step_up();

extern u8 card_result;
extern s32 card_cnt;
extern u8 name_ent_card_result_str0[];
extern u8 name_ent_card_result_str1[];
extern u8 name_ent_card_result_str2[];
extern u8 name_ent_card_result_str3[];

void name_ent_card_result(void)
{
    s32 result;
    u8 state;

    result = card_check_main();
    if (result != 0) {
        set_flag_b3_high_bit(1);
        card_state_clear();
        state = card_result;
        if (state == 1) {
            printf(name_ent_card_result_str0);
            sub_step_up();
        } else if (state == 241) {
            printf(name_ent_card_result_str1);
            goto_sub_step(192);
        } else if (state == 255) {
            printf(name_ent_card_result_str2);
            sub_sub_step_up();
        }
    } else {
        if ((u32)card_cnt >= 513) {
            set_flag_b3_high_bit(1);
            card_state_clear();
            printf(name_ent_card_result_str3);
            sub_sub_step_up();
        }
    }
}
