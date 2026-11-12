/* Address: 0x8007EE14 */
/* Size: 40 bytes */
/* Region: GCC (0x80041000-0x80080FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void indexed_byte_toggle(s32 arg);
extern void sub_sub_step_up(void);

void indexed_toggle_step(void) {
    indexed_byte_toggle(1);
    sub_sub_step_up();
}
