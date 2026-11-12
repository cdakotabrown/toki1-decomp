/* Address: 0x801332B4 (true start 0x801332A0, .sym late by 5 insns) */
/* @SymbolStart: 0x801332A0 */
/* @SymbolSize: 64 */
#include "types.h"

extern void sub_sub_step_up(void);

extern s32 serifu_no;
extern s16 shugaku_birth_now_serifu;
extern u8 basho_num;

void shugaku_birth_sel_serifu(void)
{
    shugaku_birth_now_serifu = (s16)(serifu_no * 2 + 13);
    basho_num = 2;
    sub_sub_step_up();
}
