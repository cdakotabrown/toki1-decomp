/* @SymbolStart: 0x80138804 */
/* @SymbolSize: 96 */
/* kangei_birth_choice_step2 - KANGEI overlay, Address: 0x8013880C, Size: 88 bytes (22 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern u8 now_sub_sub_step;
extern u8 kangei_icon_flag;
extern s16 kangei_now_s;
extern s32 serifu_no;

void kangei_birth_choice_step2(void)
{
    s32 hidden_t6 = serifu_no;

    if (hidden_t6 != 0) {
        kangei_icon_flag = 1;
        now_sub_sub_step = (u8)(now_sub_sub_step + 57);
    } else {
        kangei_now_s = (s16)(kangei_now_s + 1);
        sub_sub_step_up();
    }
}
