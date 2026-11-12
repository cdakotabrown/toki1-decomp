/* Address: 0x80132E54 (true start 0x80132E20, .sym late by 13 insns) */
/* @SymbolStart: 0x80132E20 */
/* @SymbolSize: 100 */
#include "types.h"

extern void shugaku_text_set(void);
extern void sub_sub_step_up(void);

extern s16 shugaku_birth_last_serifu;
extern s16 shugaku_birth_now_serifu;
extern s32 shugaku_birth_last_ss_serifu;
extern s32 shugaku_birth_ss_serifu;
extern s32 shugaku_birth_last_ss_serifu_kind;
extern s32 shugaku_birth_ss_serifu_kind;
extern s32 shugaku_birth_last_sss_sound;
extern s32 shugaku_birth_sss_sound;

void shugaku_birth_serifu_pop(void)
{
    shugaku_birth_now_serifu = shugaku_birth_last_serifu;
    shugaku_birth_ss_serifu = shugaku_birth_last_ss_serifu;
    shugaku_birth_ss_serifu_kind = shugaku_birth_last_ss_serifu_kind;
    shugaku_birth_sss_sound = shugaku_birth_last_sss_sound;
    shugaku_text_set();
    sub_sub_step_up();
}
