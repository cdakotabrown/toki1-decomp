/* Likely Symbol: sprite_fade (from o.bin, offset +188 bytes from 0x8007ECF0) */
/* Address: 0x8007EDAC */
/* Size: 52 bytes */
/* Compiler: IDO 5.2 with frame patching */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 ba_string;

extern void k_reset(s32 arg0);
extern void basyo_init(u8 *arg0);
extern void sub_sub_step_up(void);

void basyo_string_step(void) {
    k_reset(0);
    basyo_init(&ba_string);
    sub_sub_step_up();
}
