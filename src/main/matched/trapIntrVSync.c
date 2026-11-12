/* Function: trapIntrVSync (libetc trapIntrVSync, the VSync interrupt handler) */
/* Address: 0x800B437C (true start; symbol recorded at 0x800B4398) */
/* Size: 72 bytes (18 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --konami-lib-sched */
/* @SymbolStart: 0x800B437C */
/* @SymbolSize: 72 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: maspsx --aspsx-version=2.67 */
#include "types.h"

extern volatile s32 Vcount;
extern void (* volatile libapi_vsync_callback)(void);

void trapIntrVSync(void) {
    void (*cb)(void);

    Vcount += 1;
    cb = libapi_vsync_callback;
    (void)Vcount;
    if (cb != 0) {
        cb();
    }
}
