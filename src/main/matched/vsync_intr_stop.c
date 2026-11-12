/* Address: 0x800B4344 */
/* Size: 56 bytes (14 instructions) */
/* Region: Library (>= 0x80086000) */
/* `@Compiler: ido53`, a compiler this project does not build; that was */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s16 libapi_vsync_intr_enabled;
extern void vsync_intr_table_clear(s16 *arg0, s32 arg1);
extern void InterruptCallback(s32 arg0, s32 arg1);

void vsync_intr_stop(void) {
    s16 *p;

    p = &libapi_vsync_intr_enabled;
    *p = 0;
    vsync_intr_table_clear(p, 2);
    InterruptCallback(0, 0);
}
