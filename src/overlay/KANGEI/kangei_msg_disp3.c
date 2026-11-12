/* @SymbolStart: 0x80136E5C */
/* @SymbolSize: 96 */
#include "types.h"

extern s32 msg_str_disp(s16 a0, s16 a1, u8 a2, void *a3, s32 a4, void *a5, void *a6, u8 a7);
extern void sub_sub_step_up(void);

extern u32 kangei_msg3_suf_str;
extern u32 kangei_msg3_bul_str;
extern s32 kangei_ss_s;

void
kangei_msg_disp3(void)
{
    msg_str_disp(-86, 64, 1, ((void ***)kangei_ss_s)[13][3], 0,
                  &kangei_msg3_suf_str, &kangei_msg3_bul_str, 0);
    sub_sub_step_up();
}
