/* Address: 0x80062808 */
/* Size: 64 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u32 step_counter;
extern void cursor_elem(void);
extern void sub_step_up(void);

void title_elem(void) {
    cursor_elem();
    if (step_counter >= 33) {
        sub_step_up();
    }
}
