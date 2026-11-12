/* Address: 0x80074854 */
/* Size: 60 bytes (15 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x8007484C */
/* @SymbolSize: 68 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 schedule_cursor;

extern void class_clear_n(s32);
extern void goto_step(s32);

void week_day_exit0(void) {
    schedule_cursor++;
    class_clear_n(96);
    class_clear_n(97);
    goto_step(49);
}
