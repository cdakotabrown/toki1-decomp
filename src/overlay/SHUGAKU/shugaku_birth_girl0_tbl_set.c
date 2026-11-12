/* @SymbolSize: 104 */
#include "types.h"

extern void shugaku_tbl_init(void);
extern void sub_sub_step_up(void);

extern s32 shugaku_birth_ss_serifu;
extern s32 shugaku_birth_ss_serifu_kind;
extern s32 shugaku_birth_sss_sound;
extern s16 shugaku_birth_now_serifu;
extern s16 shugaku_birth_now_serifu_in;
extern s32 shugaku_pbirth_serifu_tbl[];
extern s32 shugaku_pbirth_serifu_kind_tbl[];
extern s32 shugaku_pbirth_sound_tbl[];

void shugaku_birth_girl0_tbl_set(void)
{
    shugaku_tbl_init();
    shugaku_birth_ss_serifu = shugaku_pbirth_serifu_tbl[1];
    shugaku_birth_ss_serifu_kind = shugaku_pbirth_serifu_kind_tbl[1];
    shugaku_birth_sss_sound = shugaku_pbirth_sound_tbl[1];
    shugaku_birth_now_serifu = 1;
    shugaku_birth_now_serifu_in = 0;
    sub_sub_step_up();
}
