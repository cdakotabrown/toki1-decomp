/* Address: 0x800877B0 */
/* Size: 120 bytes (30 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc272 */
/* @CompileFlags: --fill-epilogue */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 CD_init(void);
extern void CdSyncCallback(void *);
extern void CdReadyCallback(void *);
extern void CdReadCallback(void *);
extern void printf(void *);
extern void def_cbsync();
extern void def_cbready();
extern void def_cbread();
extern s32 libcd_cdinit_fail_msg;

s32 CdInit(void) {
    s32 i;

    i = 4;
loop:
    if (CD_init() == 0) {
        goto ok;
    }
    i--;
    if (i != -1) {
        goto loop;
    }
    printf(&libcd_cdinit_fail_msg);
    return 0;
ok:
    CdSyncCallback(def_cbsync);
    CdReadyCallback(def_cbready);
    CdReadCallback(def_cbread);
    return 1;
}
