/* Likely Symbol: holiday_club (from o.bin, offset +124 bytes from 0x80078968) */
/* Address: 0x800788EC */
/* Size: 64 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 pad_trig;
extern void class_clear_f(void);
extern void goto_sub_step(s32 arg0);

void holiday_taibu_club_exit(void) {
    class_clear_f();
    if (pad_trig & 0x60) {
        goto_sub_step(0);
    }
}
