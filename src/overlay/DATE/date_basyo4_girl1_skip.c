/* @SymbolStart: 0x8013D33C */
/* @SymbolSize: 136 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=1 */
/* Address: 0x8013D33C (cross-prologue head) / 0x8013D344 (body) */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

extern u8 now_girl;
extern u8 now_month;
extern s32 serifu_no;

extern s32 sub_sub_step_up(void);

s32 date_basyo4_girl1_skip(void) {
    u32 charId;
    s32 one1;
    s32 one2;

    one1 = 1;
    if (now_girl != one1) {
        goto skip_to_advance;
    }

    charId = (u32)now_month;
    if (charId < 6) {
        goto skip_to_advance;
    }
    if (charId >= 9) {
        goto skip_to_advance;
    }

    one2 = 1;
    if (serifu_no != one2) {
        goto skip_to_advance;
    }

    sub_sub_step_up();
    return 0;

skip_to_advance:
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
}
