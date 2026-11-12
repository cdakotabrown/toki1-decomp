/* Address: 0x80060B68 */
/* Size: 40 bytes */
/* Region: GCC (0x80041000-0x80080FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void class_clear_f(void);
extern void goto_step(s32 arg);

void join_club_exit(void) {
    class_clear_f();
    goto_step(48);
}
