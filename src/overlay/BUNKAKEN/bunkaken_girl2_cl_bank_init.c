/* @SymbolSize: 100 */
/* Address: 0x80142874 */
/* Size: 100 bytes (25 insns) + 7 trailing words. */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 date_name_set(s32 a0);
extern void bunkaken_girl2_cl_bank_set(void);

extern s32 bunkaken_bank_ptr;
extern s32 bunkaken_bank_ptr_1;
extern s32 bunkaken_bank_ptr_2;
extern volatile s16 now_serifu_in;
extern volatile s32 ss_serifu;
extern volatile s32 ss_serifu_kind;
extern volatile s32 sss_sound;

void bunkaken_girl2_cl_bank_init(void) {
    s32 a, b, c;
    date_name_set(2);
    bunkaken_girl2_cl_bank_set();
    a = bunkaken_bank_ptr;
    b = bunkaken_bank_ptr_1;
    c = bunkaken_bank_ptr_2;
    now_serifu_in = 0;
    ss_serifu = a;
    ss_serifu_kind = b;
    sss_sound = c;
    sub_sub_step_up();
}
