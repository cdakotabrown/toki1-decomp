/* @SymbolStart: 0x80136EBC */
/* @SymbolSize: 104 */
#include "types.h"

extern void msg_str_disp(s32, s32, s32, s32, s32, s32, s32, s32);
extern void sub_sub_step_up(void);
extern s32 kangei_msg4_suf_str;
extern s32 kangei_msg4_bul_str;
extern s16 kangei_now_s_in;
extern s32 kangei_ss_s;

void kangei_msg_disp4(void)
{
    s32 t6_param = kangei_ss_s;

    s32 a3 = *(s32 *)(*(s32 *)(t6_param + 52) + 16);
    msg_str_disp(-44, 80, 0, a3, 0, (s32)&kangei_msg4_suf_str, (s32)&kangei_msg4_bul_str, 0);
    kangei_now_s_in = 5;
    sub_sub_step_up();
}
