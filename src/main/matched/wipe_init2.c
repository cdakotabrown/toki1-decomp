/* Function: wipe_init2 */
/* Address: 0x8007C8D4  (registered address corrected 2026-09-17; see below) */
/* Size: 52 bytes (13 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* together with `@CompileFlags: -Wc,-pic1`; `addiu sp,sp,-40` / */
/* @SymbolStart: 0x8007C8D4 */
/* @SymbolSize: 52 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s32 fade_flag;
extern u8 fade_bri;
extern void sub_sub_step_up(void);

void wipe_init2(void) {
    fade_flag = 1;
    fade_bri = 128;
    sub_sub_step_up();
}
