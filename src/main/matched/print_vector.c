/* Address: 0x8009B138 */
/* Size: 48 bytes (12 instructions) */
/* Region: Library (0x80086000+) */
/* Compiler: Psy-Q GCC 2.6.0 -O2 (@Compiler: gcc260) -- RETAGGED from IDO. */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 print_vector_tbl0;
void printf();

void print_vector(s32 *p)
{
    printf(&print_vector_tbl0, p[5], p[6], p[7]);
}
