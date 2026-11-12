/* Address: 0x8013FC54 */
/* Size: 100 bytes (25 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void date_name_set(s32 a0);
extern void bunkasai_girl3_vst_bk0_1_bank_set(void);

extern s32 bunkasai_serifu_ptr;
extern s32 bunkasai_serifu_ptr_1;
extern s32 bunkasai_serifu_ptr_2;
extern volatile s16 now_serifu_in;
extern volatile s32 ss_serifu;
extern volatile s32 ss_serifu_kind;
extern volatile s32 sss_sound;

void bunkasai_girl3_clubs0_1_srf_bank_set(void) {
    s32 a, b, c;
    date_name_set(3);
    bunkasai_girl3_vst_bk0_1_bank_set();
    a = bunkasai_serifu_ptr;
    b = bunkasai_serifu_ptr_1;
    c = bunkasai_serifu_ptr_2;
    now_serifu_in = 0;
    ss_serifu = a;
    ss_serifu_kind = b;
    sss_sound = c;
    sub_sub_step_up();
}
