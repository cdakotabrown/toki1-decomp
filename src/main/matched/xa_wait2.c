/* Address: 0x80052420 */
/* Size: 96 bytes */
/* Region: GCC (0x80041000-0x80080FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void k_disp_inc(void);
extern void k_disp_goto_line_end(void);
extern s32 check_end_k(void);

extern u32 pad_trig;

extern u8 step_mode;

void xa_wait2(void) {
    k_disp_inc();
    if ((pad_trig & 0x860) != 0) {
        k_disp_goto_line_end();
        if (check_end_k() != 0) {
            step_mode = step_mode + 1;
        }
    }
}
