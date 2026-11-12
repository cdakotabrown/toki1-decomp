/* Likely Symbol: step34_load (from o.bin, offset +92 bytes from 0x8005ADDC) */
/* Address: 0x8005A638 */
/* Size: 40 bytes (10 instructions) */
/* Region: GCC (0x80041000-0x80080FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void k_disp_inc(void);
extern void sub_sub_step_up(void);

void step34_init1(void) {
    k_disp_inc();
    sub_sub_step_up();
}
