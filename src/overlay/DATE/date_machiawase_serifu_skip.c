/* @SymbolStart: 0x8013ADA4 */
/* @SymbolSize: 88 */
/* Address: 0x8013ADA4 (cross-prologue head) / 0x8013ADAC (body) */
/* Size: 88 bytes (22 instructions) */
#include "types.h"

extern u8 date_machi_mode;
extern u8 date_machi_done;

extern void sub_sub_step_up(void);
extern s32 date_machiawase_serifu_run(void);

s32 date_machiawase_serifu_skip(void) {
    if (date_machi_mode != 0 || date_machi_done == 1) {
        sub_sub_step_up();
        sub_sub_step_up();
        return 0;
    }
    return date_machiawase_serifu_run();
}
