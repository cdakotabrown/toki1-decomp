/* Debug Symbol: timeout (from o.bin, high match) */
/* Address: 0x80086F6C */
/* Size: 56 bytes (14 instructions) */
/* Compiler: IDO 5.2 */
/* Region: IDO (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_instructions --offset1 0x08 --offset2 0x0c */
/* @PostProc: patch_ra_offset --old 20 --new 16 */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x34 */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern s32 timeout_tbl0;
extern void printf(s32 *arg0, s32 arg1);
extern void MDEC_report(void);
extern void MDEC_reset(s32 arg0);

s32 timeout(s32 arg0) {
    printf(&timeout_tbl0, arg0);
    MDEC_report();
    MDEC_reset(1);
    return 0;
}
