/* Address: 0x80132F88 (true start 0x80132F44, .sym late by 17 insns) */
/* @SymbolStart: 0x80132F44 */
/* @SymbolSize: 124 */
#include "types.h"

extern void serifu_main(s32 a0, s32 a1, s32 a2);

extern s16 shugaku_birth_now_serifu;
extern s16 shugaku_birth_now_serifu_in;
extern s32 shugaku_birth_ss_serifu;
extern s32 shugaku_birth_ss_serifu_kind;
extern s32 shugaku_birth_sss_sound;
extern volatile u8 serifu_no;

extern s32 now_s;
extern s32 now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;

void shugaku_birth_serifu_run(void)
{
    now_s = (s32)&shugaku_birth_now_serifu;
    now_s_in = (s32)&shugaku_birth_now_serifu_in;
    ss_s = shugaku_birth_ss_serifu;
    ss_s_kind = shugaku_birth_ss_serifu_kind;
    sss_so = shugaku_birth_sss_sound;
    serifu_main((s32)serifu_no, 1, 0);
}
