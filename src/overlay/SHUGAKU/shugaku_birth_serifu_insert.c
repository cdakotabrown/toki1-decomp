/* @SymbolStart: 0x80132364 */
/* @SymbolSize: 216 */
/* Address: 0x80132364 (head) / 0x8013236C (prologue) */
/* Size: 216 bytes (54 instructions) */
#include "types.h"

extern volatile u8 step_mode;
extern u8 now_sub_sub_step;
extern s32 shugaku_birth_ss_serifu;
extern s32 shugaku_birth_ss_serifu_kind;
extern s32 shugaku_birth_sss_sound;
extern s16 shugaku_birth_now_serifu;
extern s16 shugaku_birth_now_serifu_in;
extern s32 shugaku_serifu_tbl[];
extern s32 shugaku_serifu_kind_tbl[];
extern s32 shugaku_sound_tbl[];

extern void shugaku_bank_ptr_init(void);
extern void shugaku_birth_serifu_run(void);
extern void shugaku_birth_serifu_set(void);

void shugaku_birth_serifu_insert(void)
{
    s32 step;
    s32 saved;

    step = step_mode;
    if (step == 0) {
        step_mode = step + 1;
        shugaku_bank_ptr_init();
        shugaku_birth_ss_serifu = shugaku_serifu_tbl[1];
        shugaku_birth_ss_serifu_kind = shugaku_serifu_kind_tbl[1];
        shugaku_birth_sss_sound = shugaku_sound_tbl[1];
        shugaku_birth_now_serifu = 14;
        shugaku_birth_now_serifu_in = 0;
    }
    step = step_mode;
    if (step == 1) {
        saved = now_sub_sub_step;
        shugaku_birth_serifu_run();
        if (saved != now_sub_sub_step) {
            shugaku_birth_serifu_set();
            now_sub_sub_step = now_sub_sub_step - 1;
            shugaku_birth_now_serifu = 5;
        }
    }
}
