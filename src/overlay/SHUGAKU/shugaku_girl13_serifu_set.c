/* Address: 0x8013981C */
/* Size: 72 bytes (18 insns) */
#include "types.h"

extern s16 now_serifu_shugaku;
extern s32 ss_serifu_shugaku;
extern s32 ss_serifu_shugaku_kind;
extern s32 sss_sound_shugaku;
extern u8  now_sub_sub_step;
extern s32 shugaku_serifu_tbl[];
extern s32 shugaku_serifu_kind_tbl[];
extern s32 shugaku_sound_tbl[];

void shugaku_girl13_serifu_set(void) {
    now_serifu_shugaku = 0;
    ss_serifu_shugaku = shugaku_serifu_tbl[12];
    ss_serifu_shugaku_kind = shugaku_serifu_kind_tbl[12];
    sss_sound_shugaku = shugaku_sound_tbl[12];
    now_sub_sub_step = 51;
}
