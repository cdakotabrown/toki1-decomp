/* Function: startIntrVSync */
/* Address:  0x800B42D0 */
/* Size:     116 bytes (29 instructions) */
/* Region:   Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O1 --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field --patch 0x54:rt:v0:v1 --patch 0x58:rt:v0:v1 */
#include "types.h"

extern u32 *libapi_rcnt1_mode_reg_ptr;
extern s16 libapi_vsync_intr_enabled;
extern void trapIntrVSync();
extern void setIntrVSync();

extern void ChangeClearPAD(s32);
extern void ChangeClearRCnt(s32, s32);
extern void vsync_intr_table_clear(s16 *, s32);
extern void InterruptCallback(s32, u32 *);

u32 *startIntrVSync(void) {
    s16 *p;

    *libapi_rcnt1_mode_reg_ptr = 263;
    ChangeClearPAD(0);
    ChangeClearRCnt(3, 0);
    p = &libapi_vsync_intr_enabled;
    vsync_intr_table_clear(p, 2);
    InterruptCallback(0, (u32 *)trapIntrVSync);
    *p = 1;
    return (u32 *)setIntrVSync;
}
