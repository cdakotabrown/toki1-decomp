/* Likely Symbol: device_init_step (from o.bin, offset +196 bytes from 0x80084944) */
/* Address: 0x80084880 */
/* Size: 72 bytes (18 instructions) */
/* Compiler: IDO 5.2 */
/* Region: IDO (0x80081000-0x800BFFFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 ret_step;
extern u8 ret_sub_step;
extern u8 ret_sub_sub_step;

extern void goto_step(u32);
extern void goto_sub_step(u32);
extern void goto_sub_sub_step(u32);

void step_restore(void) {
    goto_step(ret_step);
    goto_sub_step(ret_sub_step);
    goto_sub_sub_step(ret_sub_sub_step);
}
