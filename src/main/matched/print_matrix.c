/* Function: print_matrix */
/* Address: 0x8009B0C8 */
/* Size: 112 bytes (28 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc272 */
/* @CompileFlags: -O2 --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern void printf(void *, s16, s16, s16, s16, s16, s16, s16, s16, s16);
extern char print_matrix_tbl0;

void print_matrix(s16 *param) {
    printf(&print_matrix_tbl0,
                  param[0], param[1], param[2],
                  param[3], param[4], param[5],
                  param[6], param[7], param[8]);
}
